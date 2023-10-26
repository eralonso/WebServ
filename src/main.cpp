/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/26 15:17:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>

#define BUFFER_SIZE 300

typedef int	socket_t;

void	printInAscii( char *str )
{
	for ( uint32_t i = 0; str[ i ] != '\0'; i++ )
	{
		std::cout << static_cast< int >( str[ i ] );
		if ( str[ i ] != '\n' && str[ i + 1 ] != '\0' )
			std::cout << " ";
		else if ( str[ i + 1 ] != '\0' )
			std::cout << "\n";
	}
	std::cout << std::endl;
}

socket_t	socketCreate( int domain, int type, int protocol )
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

struct sockaddr_in	fillSockAddr( int family, uint16_t port, uint32_t ip_addr )
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

void	bindSocket( socket_t fd, struct sockaddr_in addr )
{
	int	ret;

	ret = bind( fd, ( struct sockaddr * )&addr, sizeof( addr ) );
	if ( ret < 0 )
	{
		std::cerr << "Error: Bind socket [ " << ret << " ] && errno [ " << errno << " ] = [ " << EADDRINUSE <<  " ]" << " or [ " << EACCES << " ] or [ " << EBADF << " ]" << std::endl;
		close( fd );
		exit ( 1 );
	}
	std::cout << "Log: Socket binded" << std::endl;
}

void	listenFromSocket( socket_t fd, int backlog )
{
	if ( listen( fd, backlog ) < 0 )
		std::cerr << "Error: Listen from socket" << std::endl;
	std::cout << "Log: Starting listen " << std::endl;
}

socket_t	acceptConnection( socket_t fd )
{
	socket_t			connected;
	struct sockaddr_in	addr;
	socklen_t			addr_size;

	connected = accept( fd, ( struct sockaddr * )&addr, &addr_size );
	if ( connected < 0 )
	{
		std::cerr << "Error: Failed to accept incoming connection" << std::endl;
		close( fd );
		exit( 1 );
	}
	std::cout << "Log: Connection accepted" << std::endl;
	return ( connected );
}

std::string	getRequest( socket_t connected )
{
	char	buffer[ BUFFER_SIZE + 1 ];
	ssize_t	bytes_read;

	bytes_read = read( connected, buffer, BUFFER_SIZE );
	if ( bytes_read < 0 )
	{
		std::cerr << "Error: Failed to read request" << std::endl;
		exit( 1 );
	}
	buffer[ bytes_read ] = 0;
	//printInAscii( buffer );
	std::cout << "Log: Request readed ->" << buffer << "<-" << std::endl;
	return ( std::string( buffer, std::strlen( buffer ) ) );
}

bool	analyzeRequest( std::string request )
{
	if ( request == "finish" || request.length() == 0 )
		return ( true );
	return ( false );
}

void	sendResponse( socket_t connected, std::string response )
{
	if ( write( connected, response.c_str(), response.size() ) < 0 )
	{
		std::cerr << "Error: Failed to send response" << std::endl;
		exit( 1 );
	}
	std::cout << "Log: Response sended" << std::endl;
}

socket_t	createPassiveSocket( int port, int backlog )
{
	int					fd;
	struct sockaddr_in	addr;

	fd = socketCreate( AF_INET, SOCK_STREAM, 0 );
	addr = fillSockAddr( AF_INET, port, INADDR_ANY );
	bindSocket( fd, addr );
	listenFromSocket( fd, backlog );
	return ( fd );
}

void	setPollFd( struct pollfd *pFd, int pos, socket_t fd, int events )
{
	pFd[ pos ].fd = fd;
	pFd[ pos ].events = events;
}

int	main( void )
{
	socket_t		fd;
	int				pollRet;
	int				use = 0;
	struct	pollfd	pFds[ 10 ];
	int				port = 9375;
	int				backlog = 20;
	std::string		request;
	std::string		response = "<HTML><BODY>\n\
<H1>301 Moved</H1>\n\
The document has moved\n\
<A HREF=\"http://www.google.com/\">here</A>.\n\
</BODY></HTML>";
	bool				finish = false;

	fd = createPassiveSocket( port, backlog );
	std::memset( pFds, 0, sizeof( struct pollfd ) );
	setPollFd( pFds, 0, fd, POLLIN );
	while ( finish == false )
	{
		pollRet = poll( pFds, 1 + use, -1 );
		if ( pollRet < 0 )
		{
			std::cerr << "Error: Poll" << std::endl;
			return ( 1 );
		}
		else if ( pollRet == 0 )
		{
			std::cerr << "Error: Poll timeout" << std::endl;
			return ( 1 );
		}
		if ( pFds[ 0 ].revents & POLLIN )
		{
			setPollFd( pFds, ++use, acceptConnection( fd ), POLLIN );
		}
		else
		{
			for ( int i = 1; i < 10; i++ )
			{
				if ( pFds[ i ].revents & POLLIN )
				{
					request = getRequest( pFds[ i ].fd );
					finish = analyzeRequest( request );
					sendResponse( pFds[ i ].fd, response );
				}
			}
		}
	}
	close( fd );
	return ( 0 );
}
