/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:23:31 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/06 08:26:34 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( Events *bEvs ): EventsTarget( bEvs ) {}

Server::~Server( void ) {}

int	Server::acceptNewClient( Event& tevent, struct sockaddr_in& addr )
{
	int					fd;
	socklen_t			size;

	( void )tevent;
	size = sizeof( addr );	
	fd = accept( socketFd, ( struct sockaddr * )&addr, &size );
		std::cerr << "Error: accept new client" << std::endl;
	this->evs->setEventRead( this, fd );
	return ( fd );
}

int	Server::onEvent( Event &tevent )
{
	struct sockaddr_in	addr;

	if ( tevent.flags & EV_ERROR )
		errx( EXIT_FAILURE, "Event error:	id: '%ld' filt: '%hd' flags: '%hd' fflags: '%ud' data:'%ld'\n",
			tevent.ident, tevent.filter, tevent.flags, tevent.fflags, tevent.data );
	else if ( tevent.filter & EVFILT_READ )
		return ( acceptNewClient( tevent, addr ) );
	return ( 0 );
}
