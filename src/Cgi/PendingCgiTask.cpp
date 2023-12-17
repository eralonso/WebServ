/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 19:05:10 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PendingCgiTask.hpp"

PendingCgiTask::PendingCgiTask( void ): request( *new Request() )
{
	this->timestamp = std::clock();
}

PendingCgiTask::PendingCgiTask( pid_t pid, Request& request, int fd ): \
									pid( pid ), request( request ), \
									fd( fd )
{
	this->timestamp = std::clock();
}

PendingCgiTask::PendingCgiTask( const PendingCgiTask &b ): \
									pid( b.pid ), request( b.request ), \
									timestamp( b.timestamp ), fd( b.fd ) {}

PendingCgiTask&	PendingCgiTask::operator=( const PendingCgiTask &b )
{
	if ( this != &b )
	{
		this->pid = b.pid;
		this->request = b.request;
		this->fd = b.fd;
		this->timestamp = b.timestamp;
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

bool	PendingCgiTask::isTimeout( std::clock_t toDuration ) const
{
	std::clock_t	now = std::clock();
	std::clock_t	duration = now - this->timestamp;

	return ( duration < toDuration );
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
		resBody += buf;
		bytes_read = read( this->fd, buf, BUFFER_SIZE );
	}
	if ( bytes_read < 0 )
		Log::Error( "Read from child failed" );
	buf[ bytes_read ] = 0;
	resBody += buf;
	close( this->fd );
	return ( resBody );
}

void	PendingCgiTask::applyTaskOutputToReq( void )
{
	char		buf[ BUFFER_SIZE + 1 ];
	std::string	resBody;
	int			bytes_read = read( this->fd, buf, BUFFER_SIZE );

	while ( bytes_read == BUFFER_SIZE )
	{
		buf[ bytes_read ] = 0;
		resBody += buf;
		bytes_read = read( this->fd, buf, BUFFER_SIZE );
	}
	close( this->fd );
	if ( bytes_read < 0 )
	{
		Log::Error( "Read from child failed" );
		buf[ 0 ] = 0;
		return ;
	}
	buf[ bytes_read ] = 0;
	resBody += buf;
	Log::Success( "read" + resBody );
	this->request.setCgiOutput( resBody );
}
