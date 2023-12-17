/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 18:43:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiExecutor.hpp"

PendingCgiTasks	CgiExecutor::pendingTasks;

CgiExecutor::CgiExecutor( Request& request, char **env ): request( request )
{
	(void)env;
	request.getClient()->cgis.findCgi( request.getDocExt(), binary );
	this->argument = "." + request.getRouteChaineString() + request.getDocument();
	Log::Info( "CgiExecutor binary: " + this->binary );
	Log::Info( "CgiExecutor argment: " + this->argument );
	// char				*envPath;
	this->argv[ 0 ] = ( char * )this->binary.c_str();
	this->argv[ 1 ] = ( char * )this->argument.c_str();
	this->argv[ 2 ] = NULL;
	this->childEnv[ 0 ] = NULL;
	this->childEnv[ 9 ] = NULL;
}

CgiExecutor::~CgiExecutor( void ) {}

void	CgiExecutor::onFailFork( void )
{
		Log::Error( "fork: failed create child process" );
		close( this->fdToChild[ FDIN ] );
		close( this->fdToChild[ FDOUT ] );
		close( this->fdFromChild[ FDIN ] );
		close( this->fdFromChild[ FDOUT ] );
		throw ;
}

void	CgiExecutor::onFailToChildPipeOpen( void )
{
	Log::Error( "pipe: failed to open pipe to child" );
	throw ;
}

void	CgiExecutor::onFailFromChildPipeOpen( void )
{
	Log::Error( "pipe: failed to open pipe from child" );
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	throw ;
}

void	CgiExecutor::onChildProcess( void )
{
		//On child
		close( this->fdToChild[ FDOUT ] );
		close( this->fdFromChild[ FDIN ] );
		if ( dup2( this->fdToChild[ FDIN ], FDIN ) < 0 )
			Log::Error( "dup2: fdToChild[0] to 0 failed: " \
					+ SUtils::longToString( errno ) );
		if ( dup2( this->fdFromChild[ FDOUT ], FDOUT ) < 0 )
			Log::Error( "dup2: fdFromChild[1] to 1 failed: " \
					+ SUtils::longToString( errno ) );
		close( this->fdToChild[ FDIN ] );
		close( this->fdFromChild[ FDOUT ] );
		execve( this->binary.c_str(), this->argv, this->childEnv );
		Log::Error( "execve: Not found binary " + this->binary );
		return ; //exit(1) not allowed? No :(
}

void	CgiExecutor::onParentProcess( pid_t childPid )
{
	std::string	body = this->request.getBody();
	body = std::string( "Dommy body" );
	char		*reqBody = ( char * )body.c_str();
	size_t		reqBodySize = body.length();

	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	write( this->fdToChild[ FDOUT ], reqBody, reqBodySize );
	close( this->fdToChild[ FDOUT ] );
	PendingCgiTask	task( childPid, this->request, this->fdFromChild[ FDIN ] );
	CgiExecutor::pendingTasks.appendTask( task );
	CgiExecutor::attendPendingCgiTasks();
}

std::string	CgiExecutor::getChildOutput( PendingCgiTask *task )
{
	if ( !task )
		return ( "" );
	return ( task->getTaskOutput() );
}

int	CgiExecutor::execute( void )
{
	pid_t	pid;

	if ( pipe( this->fdToChild ) )
		onFailToChildPipeOpen();
	// Log::Success("Pipe To Child Created on fds: " + SUtils::longToString(fdToChild[0]) + "," + SUtils::longToString(fdToChild[1]));
	if ( pipe( this->fdFromChild ) )
		onFailFromChildPipeOpen();
	// Log::Success("Pipe From Child Created on fds: " + SUtils::longToString(fdFromChild[0]) + "," + SUtils::longToString(fdFromChild[1]));
	// Log::Success("Request " + SUtils::longToString (request.getClient()->getId()));
	// Log::Success("with client " + SUtils::longToString (request.getClient()->getId()));
	// Log::Success("using socket " + SUtils::longToString (request.getClient()->getClientSocket()));
	pid = fork();
	if ( pid < 0 )
		onFailFork();
	if ( pid == 0 )
		onChildProcess();
	onParentProcess( pid );
	return ( 0 );
	// return getChildOutput();
}

PendingCgiTask	*CgiExecutor::getCompletedTask( void )
{
	pid_t	pid;

	pid = waitpid( -1, NULL, WNOHANG );
	if ( pid < 1 )
		return ( NULL );
	return ( &( CgiExecutor::pendingTasks[ pid ] ) );
}

PendingCgiTask	*CgiExecutor::getTimeoutedTask( clock_t to )
{
	PendingCgiTasks::iterator	it = CgiExecutor::pendingTasks.begin();
	PendingCgiTasks::iterator	ite = CgiExecutor::pendingTasks.end();

	while ( it != ite )
	{
		if ( it->second.isTimeout( to ) )
			return ( &( it->second ) );
		it++;
	}
	return ( NULL );
}

std::string	CgiExecutor::getCompletedTaskOutput( void )
{
	PendingCgiTask	*task = getCompletedTask();
	std::string		ret;

	if ( task != NULL )
	{
		ret = task->getTaskOutput();
		CgiExecutor::pendingTasks.eraseTask( task->getPid() );
		return ( ret );
	}
	return ( "" );
}

size_t	CgiExecutor::purgeTimeoutedTasks( clock_t to, size_t max )
{
	size_t			i = 0;
	PendingCgiTask	*task = NULL;

	while ( i < max && ( task = getTimeoutedTask( to ) ) )
	{
		CgiExecutor::pendingTasks.eraseTask( task->getPid() );
		i++;
	}
	return ( i );
}

void	CgiExecutor::attendPendingCgiTasks( void )
{
	PendingCgiTask	*pTask = NULL;
	Client			*cli = NULL;

	while ( ( pTask = CgiExecutor::getCompletedTask() ) != NULL )
	{
		Log::Info( "Cgi Task completed" );
		Request&	req = pTask->getRequest();
		pTask->applyTaskOutputToReq();
		// Log::Info("CgiExecutor::attendPendingCgiTasks got:");
		// Log::Info("Request addr: " + SUtils::longToString((long)&req));
		// Log::Info("Request id: " + SUtils::longToString((long)req.
		// getId()));
		// req->getDocExt();
		cli = req.getClient();
		//req->setCgiOutput(pTask->getTaskOutput());
		// req.logStatus();
		// Log::Info("Set Cgi Request " + SUtils::longToString(req.getId()) + " ReadyToSend");
		req.setReadyToSend();
		// req.logStatus();
		if ( pTask )
		{
			CgiExecutor::pendingTasks.eraseTask( pTask->getPid() );
		}
		if ( cli != NULL )
		 	cli->allowPollWrite( true );
		cli = NULL;
	}
	// req = NULL;
	// cli = NULL;
	// while ( ( pTask = CgiExecutor::getTimeoutedTask( CGI_TO ) ) != NULL )
	// {
	// 	req = pTask->getRequest();
	// 	if ( req != NULL )
	// 	{
	// 		req->setError( 500 );
	// 		req->setReadyToSend();
	// 		cli = req->getClient();		
	// 	}
	// 	if ( pTask != NULL )
	// 	{
	// 		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
	// 	}
	// 	if ( cli != NULL )
	// 		cli->allowPollWrite( true );
	// 	req = NULL;
	// 	cli = NULL;
	// }
}
