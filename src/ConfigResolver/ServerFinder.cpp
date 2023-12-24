/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:00:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/24 12:27:25 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ServerFinder.hpp>

ServerFinder::ServerFinder( void ) {}

ServerFinder::~ServerFinder( void ) {}

const Server	*ServerFinder::find( const ServersVector& servers, \
									std::string host, std::string port )
{
	const Server	*s = NULL;

	s = getStrongServerMatch( servers, host, port );
	if ( s == NULL )
		s = getWeakServerMatch( servers, host, port );
	return ( s );
}

const Server	*ServerFinder::find( const Request& request )
{
	std::string		host;
	std::string		port;
	Client			*cli = request.getClient();
	const Server	*s = NULL;

	if ( !cli )
		return ( NULL );
	request.getHostPort( host, port );
	s = getStrongServerMatch( cli->getServers(), host, port );
	if ( s == NULL )
		s = getWeakServerMatch( cli->getServers(), host, port );
	return ( s );
}

const Server	*ServerFinder::getStrongServerMatch( const ServersVector& servers, \
	   											std::string host, \
												std::string	port )
{
	ServersVector::const_iterator	it;
	ServersVector::const_iterator	ite;
	struct sockaddr_in				addr;

	ite = servers.end();
	addr = Sockets::codeHostToAddrin( host.c_str(), SUtils::atoi( port ) );
	for ( it = servers.begin(); it != ite ; it++ )
	{
		if ( it->strongServerMatch( host, port, addr.sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	return ( NULL );
}

const Server	*ServerFinder::getWeakServerMatch( const ServersVector& servers, \
	   											std::string host, \
												std::string	port )
{
	ServersVector::const_iterator	it;
	ServersVector::const_iterator	ite;
	struct sockaddr_in				addr;

	ite = servers.end();
	addr = Sockets::codeHostToAddrin( host.c_str(), SUtils::atoi( port ) );
	for ( it = servers.begin(); it != ite ; it++ )
	{
		if ( it->weakServerMatch( host, port, addr.sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	return ( NULL );
}
