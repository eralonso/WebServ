/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:38:14 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 11:57:16 by eralonso         ###   ########.fr       */
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

	SUtils::memset( &addr, 0, sizeof( addr ) );
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
		throw std::logic_error( "Bind socket [ " \
					+ SUtils::longToString( ret ) \
					+ " ]" );
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
socket_t	Sockets::acceptConnection( socket_t fd, struct sockaddr_in& addr )
{
	socket_t	connected;
	socklen_t	addr_size;

	addr_size = sizeof( addr );
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

struct addrinfo Sockets::fillAddrinfo( int family, int socktype, \
							int protocol, int flags )
{
	struct addrinfo	info;
	
	SUtils::memset( &info, 0, sizeof( info ) );
	info.ai_family = family;
	info.ai_socktype = socktype;
	info.ai_protocol = protocol;
	info.ai_flags = flags;
	return ( info );
}

struct sockaddr	Sockets::codeHostToAddr( const char *host, int port )
{
	struct addrinfo		hints;
	struct addrinfo		*res = NULL;
	struct sockaddr_in	addr_in;
	struct sockaddr		addr;

	hints = fillAddrinfo( AF_INET, SOCK_STREAM, IPPROTO_TCP, 0 );
	getaddrinfo( host, SUtils::longToString( port ).c_str(), &hints, &res );
	if ( res != NULL )
	{
		addr = *res->ai_addr;
		freeaddrinfo( res );
	}
	else
	{
		addr_in = fillSockAddr( AF_INET, port, Binary::codeAddress( host ) );
		addr = *( ( struct sockaddr * )&addr_in );
	}
	Log::Info( "Address -> " + Binary::decodeAddress( ntohl( ( ( \
		( struct sockaddr_in * )&addr ) )->sin_addr.s_addr ) ) );
	return ( addr );
}

struct sockaddr_in	Sockets::codeHostToAddrin( const char *host, int port )
{
	struct addrinfo		hints;
	struct addrinfo		*res = NULL;
	struct sockaddr_in	addr_in;

	hints = fillAddrinfo( AF_INET, SOCK_STREAM, IPPROTO_TCP, 0 );
	getaddrinfo( host, SUtils::longToString( port ).c_str(), &hints, &res );
	if ( res != NULL )
	{
		addr_in = *( ( struct sockaddr_in * )res->ai_addr );
		freeaddrinfo( res );
	}
	else
		addr_in = fillSockAddr( AF_INET, port, Binary::codeAddress( host ) );
	Log::Info( "Address -> " + Binary::decodeAddress( \
				ntohl( addr_in.sin_addr.s_addr ) ) );
	return ( addr_in );
}

struct sockaddr	Sockets::codeHostPassiveToAddr( const char *host, int port )
{
	struct addrinfo		hints;
	struct addrinfo		*res = NULL;
	struct sockaddr_in	addr_in;
	struct sockaddr		addr;

	hints = fillAddrinfo( AF_INET, SOCK_STREAM, IPPROTO_TCP, AI_PASSIVE );
	getaddrinfo( host, SUtils::longToString( port ).c_str(), &hints, &res );
	if ( res != NULL )
	{
		addr = *res->ai_addr;
		freeaddrinfo( res );
	}
	else
	{
		addr_in = fillSockAddr( AF_INET, port, Binary::codeAddress( host ) );
		addr = *( ( struct sockaddr * )&addr_in );
	}
	Log::Info( "Address -> " + Binary::decodeAddress( ntohl( ( ( \
		( struct sockaddr_in * )&addr ) )->sin_addr.s_addr ) ) );
	return ( addr );
}

struct sockaddr_in	Sockets::codeHostPassiveToAddrin( const char *host, int port )
{
	struct addrinfo		hints;
	struct addrinfo		*res = NULL;
	struct sockaddr_in	addr_in;

	hints = fillAddrinfo( AF_INET, SOCK_STREAM, IPPROTO_TCP, AI_PASSIVE );
	getaddrinfo( host, SUtils::longToString( port ).c_str(), &hints, &res );
	if ( res != NULL )
	{
		addr_in = *( ( struct sockaddr_in * )res->ai_addr );
		freeaddrinfo( res );
	}
	else
		addr_in = fillSockAddr( AF_INET, port, Binary::codeAddress( host ) );
	Log::Info( "Address -> " + Binary::decodeAddress( \
				ntohl( addr_in.sin_addr.s_addr ) ) );
	return ( addr_in );
}
//Create a socket and perform it to be a passive socket ( listen )
socket_t	Sockets::createPassiveSocket( std::string host, int port, \
										int backlog, struct sockaddr_in& addr )
{
	socket_t		fd;
	int				optVal;
	struct sockaddr	info;

	Log::Info( "[ Passive Socket ] -> host: " + host + " && port: " \
				+ SUtils::longToString( port ) );
	optVal = 1;
	fd = socketCreate( AF_INET, SOCK_STREAM, 0 );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof( int ) );
	info = codeHostPassiveToAddr( host.c_str(), port );
	try
	{
		bindSocket( fd, &info, sizeof( info ) );
		listenFromSocket( fd, backlog );
	}
	catch ( const std::logic_error& e )
	{
		Log::Info( "Connection closed in creation [ " \
				+ SUtils::longToString( fd ) + " ]" );
		close( fd );
		throw e;
	}
	addr = *( ( struct sockaddr_in * )&info );
	return ( fd );
}
