/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/03 17:04:47 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "Router.hpp"
#include <unistd.h>
#include "Client.hpp"
#include "CgiExecutor.hpp"

PendingCgiTasks	CgiExecutor::pendingTasks;

CgiExecutor::CgiExecutor( Request& request ):
	exceptBinNotAccess ("Binary not accessible"),
	exceptScriptNotAccess ("Script not accessible"),
	exceptOther ("Creation either fork or pipe failed"),
	request( request )
{
	//const Server	*s = NULL;

	//s = request.getServer();
	//if ( !s )
	//	return;
	//Log::Success("CgiExecutor::CgiExecutor Route Chaine: " + request.getRouteChaineString() );
	//this->binary = s->getCgiBinary( request.getDocExt(), request.getLocation() );
	// cli->servers[]
	//this->argument = "." + request.getRouteChaineString() + request.getDocument();
	//this->argument = s->getFinalPath( request.getLocation() ) 
	//				 + request.getDocument();
	this->binary = request.getCgiBinary( request.getDocExt() );
	this->argument = request.getFilePath();
	Log::Info( "CgiExecutor binary: " + this->binary );
	Log::Info( "CgiExecutor argment: " + this->argument );
	Log::Info( "Route chain: " + request.getRouteChaineString() );
	// char				*envPath;
	if (!checkFileReadable(this->argument))
	{
		Log::Error("Either not found or not readable: " + this->argument);
		throw exceptScriptNotAccess;
	}
	if (!checkFileExecutable(this->binary))
	{
		Log::Error("Either not found or not executable: " + this->binary);
		throw exceptBinNotAccess;
	}
	argv[0] = (char *)this->binary.c_str();
	argv[1] = (char *)this->argument.c_str();
	argv[2] = NULL;
	childEnv = NULL;
}

CgiExecutor::~CgiExecutor( void )
{
	envVars.clear();
	if ( childEnv != NULL )
		delete [] childEnv;
}

void	CgiExecutor::onFailFork( void )
{
	Log::Error( "fork: failed create child process" );
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	close( this->fdFromChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	throw exceptOther;
}

void	CgiExecutor::onFailToChildPipeOpen( void )
{
	Log::Error( "pipe: failed to open pipe to child" );
		throw exceptOther;
}

void	CgiExecutor::onFailFromChildPipeOpen( void )
{
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	throw exceptOther;
}

char	**CgiExecutor::getEnvVarList( void )
{
	StringVector::iterator	it = envVars.begin();
	StringVector::iterator	ite = envVars.end();
	size_t					i = 0;

	this->childEnv = new char *[ envVars.size() + 1 ];
	while ( it != ite )
	{
		this->childEnv[ i ] = ( char* )it->c_str();
		it++;
		i++;
	}
	this->childEnv[ i ] = 0;
	return ( this->childEnv );
}

void	CgiExecutor::onChildProcess( void )
{
	//On child
	getEnvVarList();
	close( this->fdToChild[ FDOUT ] );
	close( this->fdFromChild[ FDIN ] );
	if ( dup2( this->fdToChild[ FDIN ], FDIN ) < 0 )
		Log::Error( "dup2: fdToChild[ 0 ] to 0 failed: " \
			+ SUtils::longToString( errno ) );
	if ( dup2( this->fdFromChild[ FDOUT ], FDOUT ) < 0 )
		Log::Error( "dup2: fdFromChild[ 1 ] to 1 failed: " \
			+ SUtils::longToString( errno ) );
	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	execve( binary.c_str(), argv, childEnv );
	Log::Error( "execve: Not found binary " + binary);
	return; //exit(1) not allowed?
}

void	CgiExecutor::onParentProcess( pid_t childPid )
{
	std::string	body = request.getBody();
	char		*reqBody = ( char * )body.c_str();
	size_t		reqBodySize = body.length();

	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	write( this->fdToChild[ FDOUT ], reqBody, reqBodySize );
	close( this->fdToChild[ FDOUT ] );
	PendingCgiTask task( childPid, request, fdFromChild[ FDIN ] );
	CgiExecutor::pendingTasks.appendTask( task );
	attendPendingCgiTasks();
}

bool CgiExecutor::checkFileExecutable(std::string file)
{
	bool res = (access(file.c_str(), F_OK) == 0);
	res &= (access(file.c_str(), X_OK) == 0);
	return (res);
}

bool CgiExecutor::checkFileReadable(std::string file)
{
	bool res = (access(file.c_str(), F_OK) == 0);
	res &= (access(file.c_str(), R_OK) == 0);
	return (res);
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
	if ( pipe( this->fdFromChild ) )
		onFailFromChildPipeOpen();
	pid = fork();
	if ( pid < 0 )
		onFailFork();
	if ( pid == 0 )
		onChildProcess();
	onParentProcess( pid );
	return ( 0 );
}

PendingCgiTask	*CgiExecutor::getCompletedTask( void )
{
	pid_t	pid;
	
	pid = waitpid( -1, NULL, WNOHANG );
	if ( pid < 1 )
		return ( NULL );
	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	PendingCgiTask& tk = CgiExecutor::pendingTasks[ pid ];
	if ( !tk.isMarkedToDelete() )
		return ( &tk );
	return ( NULL );
}

PendingCgiTask	*CgiExecutor::getTimeoutedTask( double to )
{
	PendingCgiTasks::iterator	it = CgiExecutor::pendingTasks.begin();
	PendingCgiTasks::iterator	ite = CgiExecutor::pendingTasks.end();

	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	while ( it != ite )
	{
		if ( !it->second.isMarkedToDelete() && it->second.isTimeout( to, false ) )
			return ( &( it->second ) );
		it++;
	}
	return ( NULL );
}

PendingCgiTask *CgiExecutor::getMarkedToDeleteTask( void )
{
	PendingCgiTasks::iterator it = CgiExecutor::pendingTasks.begin();
	PendingCgiTasks::iterator ite = CgiExecutor::pendingTasks.end();

	if ( CgiExecutor::pendingTasks.empty() )
		return ( NULL );
	while ( it != ite )
	{
		if ( it->second.isMarkedToDelete() )
			return ( &( it->second ) );
		it++;
	}
	return ( NULL );
}

size_t	CgiExecutor::purgeTimeoutedTasks( double to, size_t max )
{
	size_t			i = 0;
	PendingCgiTask	*task = NULL;

	if ( CgiExecutor::pendingTasks.empty() )
		return 0;
	while ( i < max && ( task = getTimeoutedTask( to ) ) )
	{
		CgiExecutor::pendingTasks.eraseTask( task->getPid() );
		i++;
	}
	return ( i );
}

void	CgiExecutor::checkCompletedTasks( void )
{
	PendingCgiTask	*pTask = NULL;
	Client			*cli = NULL;
	Request			*req = NULL;

	while ( ( pTask = CgiExecutor::getCompletedTask() ) )
	{
		Log::Info( "Cgi Task completed" );
		req = &pTask->getRequest();
		pTask->applyTaskOutputToReq();
		CgiExecutor::pendingTasks.eraseTask(pTask->getPid());
		cli = req->getClient();
		req->setError( HTTP_OK_CODE );
		req->setReadyToSend();
		if ( cli != NULL )
			cli->allowPollWrite( true );
	}
}

void	CgiExecutor::checkTimeoutedTasks( void )
{
	PendingCgiTask	*pTask = NULL;
	Client			*cli = NULL;

	if ( ( pTask = CgiExecutor::getTimeoutedTask( CGI_TO ) ) != NULL )
	{
		Request& req = pTask->getRequest();
		pTask->isTimeout( CGI_TO, true );
		Log::Error( "Timeout of Req: " + SUtils::longToString( req.getId() ) \
			+ " process id: " + SUtils::longToString( pTask->getPid() ) );
		pTask->killPendingTask();
		CgiExecutor::pendingTasks.eraseTask( pTask->getPid() );
		//req.setUseCgi( false );
		req.setError( HTTP_GATEWAY_TIMEOUT_CODE );
		Router::checkErrorRedir( req.getError(), req );
		Router::checkErrorBody( req, req.getError() );
		req.setReadyToSend();
		cli = req.getClient();		
		if ( cli != NULL )
			cli->allowPollWrite( true );
	}
}

void	CgiExecutor::checkMarkedToDeleteTasks( void )
{
	PendingCgiTask	*pTask = NULL;

	if ( CgiExecutor::pendingTasks.size() > 0 \
		&& ( pTask = CgiExecutor::getMarkedToDeleteTask() ) != NULL ) 
	{
		Log::Error( "Delete Marked pending task for pid: " \
			+ SUtils::longToString( pTask->getPid() ) );
		CgiExecutor::pendingTasks.eraseTask( pTask->getPid() );
	}
}

void	CgiExecutor::attendPendingCgiTasks( void )
{
	checkCompletedTasks();
	checkTimeoutedTasks();
	checkMarkedToDeleteTasks();
}

size_t	CgiExecutor::getPendingTasksSize( void )
{
	return ( CgiExecutor::pendingTasks.size() );
}

int CgiExecutor::purgeDiscardedRequest(Request *req)
{
	pendingTasks.eraseTask(req);
    return 0;
}

pid_t CgiExecutor::findClientPendingPid(Client *cli)
{
	pid_t pid = pendingTasks.findPid(cli);
	if (pid > 0)
		return (pid);
    return 0;
}

void	CgiExecutor::pushEnvVar( const std::string& variable, \
								const std::string& value )
{
	this->envVars.push_back( variable + "=" + value );
}
