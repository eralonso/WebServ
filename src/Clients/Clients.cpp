/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/22 11:57:19 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients.hpp"

Clients::Clients( void ) {}

Clients::~Clients( void )
{
	Log::Error( "Calling Clients destructor" );
	for ( Clients::iterator it = this->begin(); it != this->end(); it++ )
	{
		if ( it->second != NULL )
			delete it->second;
	}
}

Clients::Clients( const Clients& c ): std::map< socket_t, Client * >( c ) {}

Clients&	Clients::operator=( const Clients& c )
{
	if ( this != &c )
		std::map< socket_t, Client * >::operator=( c );
	return ( *this );
}

Client	*Clients::newClient( socket_t socket, WSPoll& polls, \
							ServersVector& servers, struct sockaddr_in& info )
{
	Client	*cli = new Client( socket, polls, servers, info );

	if ( !cli )
		return ( NULL );
	this->insert( std::pair< socket_t, Client * >( socket, cli ) );
	return ( cli );
}

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
