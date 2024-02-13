/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 12:13:09 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "Router.hpp"
#include <unistd.h>
#include "Client.hpp"
#include "CgiExecutor.hpp"

CgiExecutor::CgiExecutor( Request& request ): request( request )
{
	this->binary = request.getCgiBinary( request.getDocExt() );
	this->argument = request.getFilePathRead();
	if (!checkFileExecutable(this->binary))
	{
		throw std::runtime_error("Either not found or not executable: " + this->binary);
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

CgiExecutor::CgiExecutor(const CgiExecutor & b): request( b.request )
{
	
}

CgiExecutor& CgiExecutor::operator=( const CgiExecutor& )
{
	return (*this); 	
}


void	CgiExecutor::onFailFork( void )
{
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	close( this->fdFromChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	throw std::runtime_error("fork: failed create child process");
}

void	CgiExecutor::onFailToChildPipeOpen( void )
{
	throw std::runtime_error("pipe: failed to open pipe to write to child");
}

void	CgiExecutor::onFailFromChildPipeOpen( void )
{
	close( this->fdToChild[ FDIN ] );
	close( this->fdToChild[ FDOUT ] );
	throw std::runtime_error("pipe: failed to open pipe to read from child");
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
	signal( SIGINT, SIG_DFL );
	signal( SIGQUIT, SIG_DFL );
	signal( SIGPIPE, SIG_DFL );
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
	Client	*cli;

	close( this->fdToChild[ FDIN ] );
	close( this->fdFromChild[ FDOUT ] );
	cli = this->request.getClient();
	if ( cli )
		cli->setEventProc( this->fdFromChild[ FDIN ], this->fdToChild[ FDOUT ], childPid );
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

int	CgiExecutor::execute( void )
{
	pid_t	pid;

	if ( pipe( this->fdToChild ) )
		onFailToChildPipeOpen();
	if ( pipe( this->fdFromChild ) )
		onFailFromChildPipeOpen();
	fcntl(this->fdFromChild[ FDIN ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdFromChild[ FDOUT ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdToChild[ FDIN ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->fdToChild[ FDOUT ], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	pid = fork();
	if ( pid < 0 )
		onFailFork();
	if ( pid == 0 )
		onChildProcess();
	onParentProcess( pid );
	return ( 0 );
}

void	CgiExecutor::pushEnvVar( const std::string& variable, \
								const std::string& value )
{
	this->envVars.push_back( variable + "=" + value );
}
