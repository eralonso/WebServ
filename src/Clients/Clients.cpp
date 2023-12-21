/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/21 12:07:59 by omoreno-         ###   ########.fr       */
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

Clients::Clients( const Clients& c ): std::map< socket_t, Client * >( c ) {}

Clients&	Clients::operator=( const Clients& c )
{
	if ( this != &c )
		std::map< socket_t, Client * >::operator=( c );
	return ( *this );
}

Client	*Clients::newClient( socket_t socket, WSPoll& polls, ServersVector& servers )
{
	Client	*cli = new Client( socket, polls, servers);

	if ( !cli )
		return ( NULL );
	this->insert( std::pair< socket_t, Client * >( socket, cli ) );
	// cli->cgis.appendCgi( "py", "/usr/bin/python" );
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

	// if (size()==0)
	// 	Log::Info("No Client to check");
	while ( it != ite )
	{
		if ( it->second != NULL )
			somePending |= it->second->checkPendingToSend();
		it++;
	}
	return ( somePending );
}
