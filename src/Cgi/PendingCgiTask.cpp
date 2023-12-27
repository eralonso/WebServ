/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/19 18:00:45 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "PendingCgiTask.hpp"
#include "PendingCgiTask.hpp"

PendingCgiTask::PendingCgiTask( void ): request( *new Request() ), \
										markedToDelete( false )
{
	this->timestamp = std::clock();
}

PendingCgiTask::PendingCgiTask( pid_t pid, Request& request, int fd ): \
									pid( pid ), request( request ), \
									fd( fd ), markedToDelete( false )
{
	this->timestamp = std::clock();
	Log::Info("Task pid " + SUtils::longToString(pid) + " time: " + SUtils::longToString(timestamp));
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

bool	PendingCgiTask::isTimeout( double toDuration, bool logInfo ) const
{
	std::clock_t	now = std::clock();
	long			duration;

	if ( logInfo )
	{
		// Log::Info("isTimeout at time: " + SUtils::longToString(now));
		// Log::Info("isTimeout over timestamp: " + SUtils::longToString(timestamp));
		Log::Info("Cgi Timeout: " + SUtils::longToString( \
			static_cast< long >( toDuration * 1000 ) ) + "ms" );
	}
	duration = now - this->timestamp;
	if ( logInfo )
		Log::Info( "exceeded at duration: " \
			+ SUtils::longToString( duration ) + "ms" );
	return ( duration > static_cast< long >( toDuration * 1000 ) );
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
		buf[BUFFER_SIZE] = 0;
		resBody += std::string(buf, bytes_read);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		Log::Error(std::string("Read from child failed"));
	buf[bytes_read] = 0;
	if (bytes_read > 0)
		resBody += std::string(buf, bytes_read);
	close(this->fd);
	return (resBody);
}

void	PendingCgiTask::applyTaskOutputToReq( void )
{
	char		buf[ BUFFER_SIZE + 1 ];
	std::string	resBody;
	int			bytes_read = read( this->fd, buf, BUFFER_SIZE );

	while ( bytes_read == BUFFER_SIZE )
	{
		buf[bytes_read] = 0;
		resBody += std::string(buf, bytes_read);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	close( this->fd );
	setMarkedToDelete(true);
	if ( bytes_read < 0 )
	{
		Log::Error( "Read from child failed" );
		buf[ 0 ] = 0;
		return ;
	}
	buf[bytes_read] = 0;
	resBody += std::string(buf, bytes_read);
	// Log::Success(std::string("\nread" + resBody));
	request.setCgiOutput(resBody);
}

void	PendingCgiTask::closeReadFd()
{
	if (fd >= 0)
		close(fd);
	fd = -1;
}

void	PendingCgiTask::killPendingTask()
{
	if (pid > 0)
	{
		markedToDelete = true;
		kill(pid, SIGKILL);
	}
	pid = 0;
	if (fd >= 0)
		close(fd);
	fd = -1;
}

void PendingCgiTask::setMarkedToDelete(bool value)
{
	markedToDelete = value;
}
