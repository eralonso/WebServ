/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/30 11:29:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "PendingCgiTask.hpp"
#include "PendingCgiTask.hpp"

PendingCgiTask::PendingCgiTask( void ): pid( 0 ), request( *new Request() ), \
										fd( 0 ), markedToDelete( false )
{
	this->timestamp = std::clock();
}

PendingCgiTask::PendingCgiTask( pid_t pid, Request& request, int fd ): \
									pid( pid ), request( request ), \
									fd( fd ), markedToDelete( false )
{
	this->timestamp = std::clock();
	// Log::Info("Task pid " + SUtils::longToString( pid )
	// 	+ " time: " + SUtils::longToString( this->timestamp ) );
}

PendingCgiTask::PendingCgiTask( const PendingCgiTask &b ): \
									pid( b.pid ), request( b.request ), \
									timestamp( b.timestamp ), fd( b.fd ), \
									markedToDelete( b.markedToDelete ) {}

PendingCgiTask&	PendingCgiTask::operator=( const PendingCgiTask &b )
{
	if ( this != &b )
	{
		this->pid = b.pid;
		this->request = b.request;
		this->fd = b.fd;
		this->timestamp = b.timestamp;
		this->markedToDelete = b.markedToDelete;
	}
	return ( *this );
}

PendingCgiTask::~PendingCgiTask( void ) {}

pid_t	PendingCgiTask::getPid( void ) const
{
	return ( this->pid );
}

Request&	PendingCgiTask::getRequest( void ) const
{
	return ( this->request );
}

std::clock_t	PendingCgiTask::getTimestamp( void ) const
{
	return ( this->timestamp );
}

bool	PendingCgiTask::isMarkedToDelete( void ) const
{
	return ( this->markedToDelete );
}

bool	PendingCgiTask::isTimeout( std::clock_t toDuration, bool logInfo ) const
{
	std::clock_t	now = std::clock();
	std::clock_t	duration;

	if ( logInfo )
	{
		Log::Error("Cgi Timeout: now" + SUtils::longToString( now ) );
		Log::Error("Cgi Timeout: timestamp" + SUtils::longToString( this->timestamp ) );
		Log::Info("Cgi Timeout: " + SUtils::longToString( toDuration ) + "ms" );
	}
	duration = now - this->timestamp;
	if ( logInfo )
		Log::Info( "exceeded at duration: " \
			+ SUtils::longToString( duration ) + "ms" );
	return ( duration > toDuration * CLOCKS_PER_SEC );
}

int	PendingCgiTask::getFd( void ) const
{
	return ( this->fd );
}

std::string	PendingCgiTask::getTaskOutput( void )
{
	char		buf[ BUFFER_SIZE + 1 ];
	std::string	resBody;
	int			bytes_read = read( this->fd, buf, BUFFER_SIZE );

	while ( bytes_read == BUFFER_SIZE )
	{
		buf[ BUFFER_SIZE ] = 0;
		resBody += std::string(buf, bytes_read);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	if ( bytes_read < 0 )
		Log::Error( "Read from child failed" );
	buf[ bytes_read ] = 0;
	if ( bytes_read > 0 )
		resBody += std::string(buf, bytes_read);
	close( this->fd );
	return ( resBody );
}

void	PendingCgiTask::applyTaskOutputToReq( void )
{
	char		buf[ BUFFER_SIZE + 1 ];
	std::string	resBody;
	int			bytes_read = read( this->fd, buf, BUFFER_SIZE );

	// Log::Success( "apply task output read" );
	while ( bytes_read == BUFFER_SIZE )
	{
		buf[ bytes_read ] = 0;
		resBody += buf;
		bytes_read = read( this->fd, buf, BUFFER_SIZE );
	}
	close( this->fd );
	setMarkedToDelete( true );
	if ( bytes_read < 0 )
	{
		Log::Error( "Read from child failed" );
		buf[ 0 ] = 0;
		return ;
	}
	buf[ bytes_read ] = 0;
	resBody += buf;
	this->request.setCgiOutput( resBody );
}

void	PendingCgiTask::closeReadFd( void )
{
	if ( this->fd >= 0 )
		close( this->fd );
	this->fd = -1;
}

void	PendingCgiTask::killPendingTask( void )
{
	if ( this->pid > 0 )
	{
		this->markedToDelete = true;
		kill( this->pid, SIGTERM );
	}
	this->pid = 0;
	if ( this->fd >= 0 )
		close( this->fd );
	this->fd = -1;
}

void	PendingCgiTask::setMarkedToDelete( bool value )
{
	this->markedToDelete = value;
}
