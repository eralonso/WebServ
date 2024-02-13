/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOnEvs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/13 12:23:39 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Receptionist.hpp>

int	Server::onNewClient( void )
{

	socket_t			clientFd;
	struct sockaddr_in	info;
	
	clientFd = Sockets::acceptConnection( this->_socketFd, info );
	if ( clientFd < 0 )
		return ( -1 );
	fcntl( clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	if ( !this->receptionist->newClient(clientFd, this->evs, \
			&this->receptionist->getServers(), info, this->receptionist ) )
	{
		Log::Error( "Failed to append Request" );
		close( clientFd );
		return ( -1 );
	}
	return ( 1 );
}

int	Server::onEvent( Event &tevent )
{
	if ( tevent.filter & EVFILT_READ )
		return ( onNewClient() );
	return ( 0 );
}
