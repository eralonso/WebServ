/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 10:19:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients.hpp"

Clients::Clients( void ) {}

Clients::~Clients( void )
{
	for ( Clients::iterator it = this->begin(); it != this->end(); it++ )
	{
		if ( it->second != NULL )
			delete it->second;
	}
}

Clients::Clients( const Clients& c ): ClientMap( c ) {}

Clients&	Clients::operator=( const Clients& c )
{
	if ( this != &c )
		ClientMap::operator=( c );
	return ( *this );
}

Client	*Clients::newClient( socket_t socket, Events *bEvs, \
							const ServersVector *servers, struct sockaddr_in& info, \
	   						Receptionist *recp )
{
	Client	*cli = new Client( socket, bEvs, servers, info, recp );

	if ( !cli )
		return ( NULL );
	this->insert( ClientMapPair( socket, cli ) );
	cli->setEventReadSocket();
	return ( cli );
}

//Client	*Clients::newClient( socket_t socket, WSPoll& polls, \
//							ServersVector& servers, struct sockaddr_in& info )
//{
//	Client	*cli = new Client( socket, polls, servers, info );
//
//	if ( !cli )
//		return ( NULL );
//	this->insert( ClientMapPair( socket, cli ) );
//	return ( cli );
//}

size_t	Clients::eraseClient( Client *cli )
{
	socket_t	socket;
	size_t		s;

	if ( cli != NULL )
	{
		socket = cli->getClientSocket();
		s = this->erase( socket );
		delete cli;
		return ( s );
	}
	return ( 0 );
}

size_t	Clients::eraseClient( socket_t socket )
{
	Client	*cli = NULL;

	try
	{
		cli = this->at( socket );
		if ( cli != NULL )
			delete cli;
	}
	catch ( const std::exception& e )
	{
		return ( 0 );
	}
	return ( this->erase( socket ) );
}

bool	Clients::checkPendingToSend( void )
{
	Clients::iterator	it = this->begin();
	Clients::iterator	ite = this->end();
	bool				somePending = false;

	while ( it != ite )
	{
		if ( it->second != NULL )
			somePending |= it->second->checkPendingToSend();
		it++;
	}
	return ( somePending );
}
