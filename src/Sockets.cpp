/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:38:14 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/27 13:14:34 by eralonso         ###   ########.fr       */
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
		std::cerr << "Error: Socket create" << std::endl;
		exit ( 1 );
	}
	std::cout << "Log: Socket create [ " << fd << " ]" << std::endl;
	return ( fd );
}

//Fill and return struct sockaddr_in with specifications passed as parameters
struct sockaddr_in	Sockets::fillSockAddr( int family, uint16_t port, uint32_t ip_addr )
{
	struct sockaddr_in	addr;

	std::memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = family;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = htonl( ip_addr );
	std::cout << "Log: Socket address filled: Port -> " << port \
		<< " && Host -> " << std::bitset< 32 >( addr.sin_port ) \
		<< " && Network -> " << ntohs( addr.sin_port ) << std::endl;
	return ( addr );
}

//Bind struct sockaddr_in with a socket
void	Sockets::bindSocket( socket_t fd, struct sockaddr_in addr )
{
	int	ret;

	ret = bind( fd, ( struct sockaddr * )&addr, sizeof( addr ) );
	if ( ret < 0 )
	{
		std::cerr << "Error: Bind socket [ " << ret << " ] && errno [ " \
					<< errno << " ] = [ " << EADDRINUSE <<  " ]" \
					<< " or [ " << EACCES << " ] or [ " << EBADF << " ]" \
					<< std::endl;
		exit ( 1 );
	}
	std::cout << "Log: Socket binded" << std::endl;
}

//Establish socket as listen mode and set maximum number of connections pending in queue
void	Sockets::listenFromSocket( socket_t fd, int backlog )
{
	if ( listen( fd, backlog ) < 0 )
		std::cerr << "Error: Listen from socket" << std::endl;
	std::cout << "Log: Starting listen " << std::endl;
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
		std::cerr << "Error: Failed to accept incoming connection" << std::endl;
		return ( -1 );
	}
	std::cout << "Log: Connection accepted [ " << connected << " ]" << std::endl;
	return ( connected );
}

//Create a socket and perform it to be a passive socket ( listen )
socket_t	Sockets::createPassiveSocket( int port, int backlog )
{
	int					fd;
	struct sockaddr_in	addr;

	fd = socketCreate( AF_INET, SOCK_STREAM, 0 );
	addr = fillSockAddr( AF_INET, port, INADDR_ANY );
	bindSocket( fd, addr );
	listenFromSocket( fd, backlog );
	return ( fd );
}
