/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/27 19:02:19 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"
#include "WSPoll.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unistd.h>

//int	getRequest( socket_t connected, std::string& request, struct pollfd *polls, int i )
//{
//	char	buffer[ BUFFER_SIZE + 1 ];
//	ssize_t	bytes_read;
//
//	bytes_read = read( connected, buffer, BUFFER_SIZE );
//	if ( bytes_read < 0 )
//	{
//		std::cerr << "Error: Failed to read request" << std::endl;
//		closeClient( polls, i );
//		return ( -1 );
//	}
//	buffer[ bytes_read ] = 0;
//	//PUtils::printInAscii( buffer );
//	std::cout << "Log: Request readed ->" << buffer << "<-" << std::endl;
//	request = std::string( buffer, std::strlen( buffer ) );
//	return ( 1 );
//}
//
//bool	analyzeRequest( std::string request )
//{
//	if ( request == "finish" || request.length() == 0 )
//		return ( true );
//	return ( false );
//}
//
//void	sendResponse( socket_t connected, std::string response )
//{
//	if ( write( connected, response.c_str(), response.size() ) < 0 )
//	{
//		std::cerr << "Error: Failed to send response" << std::endl;
//		exit( 1 );
//	}
//	std::cout << "Log: Response sended" << std::endl;
//}

int	main( void )
{
	WSPoll		polls( MAX_CLIENTS );
	int			port = 9375;
	int			backlog = 20;
	socket_t	serverFd;
	socket_t	clientFd;
	char		buffer[ BUFFER_SIZE + 1 ];


	serverFd = Sockets::createPassiveSocket( port, backlog );
	polls.addPollfd( serverFd, POLLIN, 0, SPOLLFD );
	while ( true )
	{
		if ( polls.wait( -1 ) < 0 )
			return ( 1 );
		serverFd = polls.isNewClient();
		if ( serverFd > 0 )
		{
			clientFd = Sockets::acceptConnection( serverFd );
			if ( clientFd < 0 )
				return ( 1 );
			if ( polls.addPollfd( clientFd, POLLIN, 0, CPOLLFD ) == false )
				close( clientFd );
		}
		else
		{
			clientFd = polls.getPerformClient();
			if ( clientFd > 0 )
			{
				std::memset( buffer, 0, BUFFER_SIZE );
				if ( read( clientFd, buffer, BUFFER_SIZE ) <= 0 )
				{
					polls.closePoll( clientFd );
					continue ;
				}
				write( clientFd, "Hola\n", 5 );
				std::cout << "Read: " << buffer << std::endl;
			}
		}
	}
	return ( 0 );
}
/*
int	main( void )
{
	socket_t		fd;
	int				pollRet;
	int				use = 0;
	WSPoll			polls( MAX_CLIENTS );
	struct	pollfd	polls[ MAX_CLIENTS ];
	int				port = 9375;
	int				backlog = 20;
	bool			finish = false;
	std::string		request;
	std::string		response = "<HTML><BODY>\n\
<H1>301 Moved</H1>\n\
The document has moved\n\
<A HREF=\"http://www.google.com/\">here</A>.\n\
</BODY></HTML>";

	fd = createPassiveSocket( port, backlog );
	std::memset( &polls, 0, sizeof( struct pollfd * ) );
	setPollFd( polls, fd, POLLIN, 0 );
	while ( finish == false )
	{
		pollRet = poll( polls, 1 + use, -1 );
		if ( checkPollReturn( pollRet ) <= 0 )
		{
			closePoll( polls, 1, 1 + use );
			return ( 1 );
		}
		if ( polls[ 0 ].revents & POLLIN )
			setPollFd( &polls[ ++use ], acceptConnection( fd ), POLLIN, 0 );
		else
		{
			for ( int i = 1; i < MAX_CLIENTS; i++ )
			{
				if ( polls[ i ].revents & POLLIN )
				{
					if ( getRequest( polls[ i ].fd, request, polls, i ) < 0 )
					{
						use--;
						continue ;
					}
					if ( analyzeRequest( request ) == true )
					{
						closeClient( polls, i );
						use--;
					}
					else
						sendResponse( polls[ i ].fd, response );
				}
			}
		}
	}
	close( fd );
	return ( 0 );
}*/
