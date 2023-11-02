/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/02 15:19:10 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"
#include "WSPoll.hpp"
#include "Utils.hpp"
#include <iostream>
#include <unistd.h>

bool	isSig = false;

void	sighandler( int )
{
	Log::Info( "Signal detected" );
	isSig = true;
}

void	signalHandler( void )
{
	signal( SIGINT, sighandler );
	signal( SIGQUIT, sighandler );
}

int	addNewClient( socket_t serverFd, WSPoll& polls )
{
	socket_t	clientFd;


	clientFd = Sockets::acceptConnection( serverFd );
	if ( clientFd < 0 )
		return ( -1 );
	if ( polls.addPollfd( clientFd, POLLIN, 0, CPOLLFD ) == false )
	{
		Log::Error( "Too many clients trying to connect to server" );
		close( clientFd );
	}
	return ( 1 );
}

std::string	getHtml( void )
{
	std::string	html;

	html = "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "\t<meta charset=\"UTF-8\">\n";
	html += "\t<title>ª</title>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "\t<h1 style=\"color: #00FFFF;\">Message from server</h1>\n";
	html += "\n";
	html += "</body>\n";
	html += "</html>";
	return ( html );
}

std::string	getHeader( void )
{
	std::string	header;

	header += "HTTP/1.1 200 OK\r\n";
	header += "Server: OREginx\r\n";
	header += "Content-Length: ";
	header += SUtils::longToString( getHtml().length() );
	header += "\r\n";
	header += "Content-Type: text/html\r\n";
	header += "\r\n";
	return ( header );
}

std::string	getResponse( void )
{
	return ( getHeader() + getHtml() );
}

void	sendResponse( socket_t connected, std::string response )
{
	if ( send( connected, response.c_str(), response.size(), 0 ) < 0 )
	{
		Log::Error( "Failed to send response" );
		exit( 1 );
	}
	Log::Success( "Response sended [ " + SUtils::longToString( connected ) + " ]" );
}

int	readRequest( socket_t clientFd, std::string& readed )
{
	char			buffer[ BUFFER_SIZE + 1 ];

	std::memset( buffer, 0, BUFFER_SIZE + 1 );
	if (recv( clientFd, buffer, BUFFER_SIZE, 0 ) <= 0 )
		return ( -1 );
	readed = buffer;
	return ( 1 );
}

void	manageClient( socket_t clientFd, WSPoll& polls )
{
	struct pollfd	*clientPoll;
	std::string		readed;

	try
	{
		clientPoll = &polls[ clientFd ];
	}
	catch ( std::out_of_range& e ) { return ; }
	if ( clientPoll->revents & POLLIN )
	{
		if ( readRequest( clientFd, readed ) < 0 )
		{
			polls.closePoll( clientFd );
			return ;
		}
		Log::Info( "Readed [ " \
				+ SUtils::longToString( clientFd )\
				+ " ]: " \
				+ readed);
		clientPoll->events |= POLLOUT;
	}
	else if ( clientPoll->revents & POLLOUT )
	{
		sendResponse( clientFd, getResponse() );
		clientPoll->events &= ~POLLOUT;
	}
}

int	main( void )
{
	WSPoll			polls( MAX_CLIENTS );
	int				port = 9375;
	int				backlog = 20;
	socket_t		serverFd;
	socket_t		clientFd;

	Binary::printInOctets( 12 );
	signalHandler();
	serverFd = Sockets::createPassiveSocket( port, backlog );
	polls.addPollfd( serverFd, POLLIN, 0, SPOLLFD );
	while ( true )
	{
		if ( isSig == true )
			return ( 1 );
		Log::Info( "Waiting for any fd ready to I/O" );
		if ( polls.wait( -1 ) < 0 )
			return ( 1 );
		serverFd = polls.isNewClient();
		if ( serverFd > 0 )
		{
			if ( addNewClient( serverFd, polls ) < 0 )
				return ( 1 );
		}
		else
		{
			clientFd = polls.getPerformClient();
			if ( clientFd > 0 )
				manageClient( clientFd, polls );
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
