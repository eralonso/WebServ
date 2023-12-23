/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:00:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/23 19:22:51 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ServerFinder.hpp>

ServerFinder::ServerFinder( void )
{
}

ServerFinder::~ServerFinder()
{
}

const Server *ServerFinder::find(const ServersVector& servers, std::string host, std::string port)
{
	struct sockaddr					info;
	struct sockaddr_in				*addr;
	ServersVector::const_iterator	it;
	ServersVector::const_iterator	ite = servers.end();

	info = Sockets::codeHost( host.c_str(), SUtils::atoi( port ) );
	addr = ( struct sockaddr_in * )&info;
	for (it = servers.begin(); it != ite ; it++)
	{
		if ( it->strongServerMatch( host, port, addr->sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	for (it = servers.begin(); it != ite ; it++)
	{
		if ( it->weakServerMatch( host, port, addr->sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	Log::Error( "Estamos jodidos" );
	return ( NULL );
}

const Server *ServerFinder::find(const Request& request)
{
	std::string			host;
	std::string			port;
	struct sockaddr		info;
	struct sockaddr_in	*addr;
	Client				*cli = request.getClient();

	if ( !cli )
		return ( NULL );
	ServersVector::const_iterator	it;
	ServersVector::const_iterator	ite = cli->getServers().end();
	request.getHostPort( host, port );
	info = Sockets::codeHost( host.c_str(), SUtils::atoi( port ) );
	addr = ( struct sockaddr_in * )&info;
	for (it = cli->getServers().begin(); it != ite ; it++)
	{
		if ( it->strongServerMatch( host, port, addr->sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	for (it = cli->getServers().begin(); it != ite ; it++)
	{
		if ( it->weakServerMatch( host, port, addr->sin_addr.s_addr ) == true )
			return ( &( *it ) );
	}
	Log::Error( "Estamos jodidos" );
	return ( NULL );
}
