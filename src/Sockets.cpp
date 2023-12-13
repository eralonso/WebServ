/* ************************************************************************* */
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
		throw std::logic_error( "Socket create-> domain: " \
				+ SUtils::longToString( domain ) \
				+ " && type: " \
				+ SUtils::longToString( type ) \
				+ " && protocol: " \
				+ SUtils::longToString( protocol ) );
				
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
void	Sockets::bindSocket( socket_t fd, struct sockaddr *addr, socklen_t len )
{
	int	ret;

	ret = bind( fd, addr, len );
	if ( ret < 0 )
	{
		throw std::logic_error( "Bind socket [ " \
					+ SUtils::longToString( ret ) \
					+ " ]" );
	}
	Log::Success( "Socket binded [ " \
			+ SUtils::longToString( fd ) \
			+ " ]" );
}

//Establish socket as listen mode and set maximum number of connections pending in queue
void	Sockets::listenFromSocket( socket_t fd, int backlog )
{
	if ( listen( fd, backlog ) < 0 )
		throw std::logic_error( "Listen from socket[ " \
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

void	Sockets::codeHost( socket_t fd, int port, std::string host )
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct sockaddr_in	addr;

	res = NULL;
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME | AI_IDN | AI_CANONIDN;
	getaddrinfo( host.c_str(), NULL, &hints, &res );
	if ( res != NULL )
	{
		bindSocket( fd, res->ai_addr, res->ai_addrlen );
		freeaddrinfo( res );
	}
	else
	{
		addr = fillSockAddr( AF_INET, port, Binary::codeAddress( host ) );
		bindSocket( fd, ( struct sockaddr * )&addr, sizeof( addr ) );
	}
}

//Create a socket and perform it to be a passive socket ( listen )
socket_t	Sockets::createPassiveSocket( std::string host, int port, int backlog )
{
	int					fd;
	int					optVal;
	//struct sockaddr_in	addr;

	Log::Error( "[ Passive Socket ] -> host: " + host + " && port: " + SUtils::longToString( port ) );
	optVal = 1;
	fd = socketCreate( AF_INET, SOCK_STREAM, 0 );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof( int ) );
	codeHost( fd, port, host );
	//addr = fillSockAddr( AF_INET, port, INADDR_ANY );
	//addr = fillSockAddr( AF_INET, port, Binary::codeAddress( "127.0.0.1" ) );
	//addr = fillSockAddr( AF_INET, port, codeHost( host ) );
	//bindSocket( fd, addr );
	listenFromSocket( fd, backlog );
	return ( fd );
}
