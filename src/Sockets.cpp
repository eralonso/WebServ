/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:38:14 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/27 11:00:44 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"

//OCCF = Orthodox canonical class form

//OCCF: Default constructor
Sockets::Sockets( void ) {}

//OCCF: Copy constructor
Sockets::Sockets( const Sockets& socket )
{
	( void ) socket;
}

//OCCF: Default destructor
Sockets::~Sockets( void ) {}

//OCCF: Assignment operator
Sockets	Sockets::operator=( const Sockets& socket )
{
	( void ) socket;
	return ( *this );
}

//Create a new socket
socket_t	Sockets::socketCreate( int domain, int type, int protocol )
{
	socket_t	fd;

	fd = socket( domain, type, protocol );
	if ( fd < 0 )
	{
		Log::Error( "Socket create" );
		exit ( 1 );
	}
	Log::Success( "Socket create [ " \
				+ SUtils::longToString( fd ) \
				+ " ]" );
	return ( fd );
}

//Fill and return struct sockaddr_in with specifications passed as parameters
struct sockaddr_in	Sockets::fillSockAddr( int family, uint16_t port, uint32_t ip_addr )
{
	struct sockaddr_in	addr;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = family;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = htonl( ip_addr );
	return ( addr );
}

//Bind struct sockaddr_in with a socket
void	Sockets::bindSocket( socket_t fd, struct sockaddr_in addr )
{
	int	ret;

	ret = bind( fd, ( struct sockaddr * )&addr, sizeof( addr ) );
	if ( ret < 0 )
	{
		Log::Error( "Error: Bind socket [ " \
					+ SUtils::longToString( ret ) \
					+ " ]" );
		exit ( 1 );
	}
	Log::Success( "Socket binded [ " \
			+ SUtils::longToString( fd ) \
			+ " ]" );
}

//Establish socket as listen mode and set maximum number of connections pending in queue
void	Sockets::listenFromSocket( socket_t fd, int backlog )
{
	if ( listen( fd, backlog ) < 0 )
		Log::Error( "Listen from socket[ " \
				+ SUtils::longToString( fd ) \
				+ " ]" );
	Log::Success( "Starting listen [ " \
			+ SUtils::longToString( fd ) \
			+ " ]" );
}

//Accept a connection from socket
socket_t	Sockets::acceptConnection( socket_t fd )
{
	socket_t			connected;
	struct sockaddr_in	addr;
	socklen_t			addr_size;

	connected = accept( fd, ( struct sockaddr * )&addr, &addr_size );
	if ( connected < 0 )
	{
		Log::Error( "Failed to accept incoming connection" );
		return ( -1 );
	}
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	Log::Success( "Connection accepted [ " \
			+ SUtils::longToString( connected ) \
			+ " ]" );
	Log::Info( "addr.sin_addr.s_addr [ decode ] -> " \
		   		+ Binary::decodeAddress( ntohl( addr.sin_addr.s_addr ) ) );
	return ( connected );
}

//Create a socket and perform it to be a passive socket ( listen )
socket_t	Sockets::createPassiveSocket( int port, int backlog )
{
	int					fd;
	int					optVal;
	struct sockaddr_in	addr;

	optVal = 1;
	fd = socketCreate( AF_INET, SOCK_STREAM, 0 );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof( int ) );
	//addr = fillSockAddr( AF_INET, port, INADDR_ANY );
	addr = fillSockAddr( AF_INET, port, Binary::codeAddress( "127.0.0.1" ) );
	bindSocket( fd, addr );
	listenFromSocket( fd, backlog );
	return ( fd );
}
