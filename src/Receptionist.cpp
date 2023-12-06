/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:44:28 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/06 10:58:43 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Receptionist.hpp>
#include <Response.hpp>

Receptionist::Receptionist(int port, int backlog, int timeout):
	polls(MAX_CLIENTS), port(port), backlog(backlog), timeout(timeout)
{
	socket_t		serverFd;
	serverFd = Sockets::createPassiveSocket( this->port, this->backlog );
	polls.addPollfd( serverFd, POLLIN, 0, SPOLLFD );
}

Receptionist::~Receptionist()
{
}

Receptionist::Receptionist(const Receptionist& b) :
	polls((const WSPoll &)b.polls)
{
	port = b.port;
	backlog = b.backlog;
	timeout = b.timeout;
}

Receptionist& 	Receptionist::operator=(const Receptionist& b)
{
	polls = WSPoll((const WSPoll &)b.polls);
	port = b.port;
	backlog = b.backlog;
	timeout = b.timeout;
	return *this;
}

int	Receptionist::sendResponse( socket_t connected, std::string response )
{
	if ( send( connected, response.c_str(), response.size(), 0 ) < 0 )
	{
		Log::Error( "Failed to send response" );
		exit( 1 );
	}
	Log::Success( "Response sended [ " + SUtils::longToString( connected ) + " ]" );
	// Log::Success( response );
	return (1);
}

int	Receptionist::readRequest( socket_t clientFd, std::string& readed )
{
	char			buffer[ BUFFER_SIZE + 1 ];

	std::memset( buffer, 0, BUFFER_SIZE + 1 );
	if (recv( clientFd, buffer, BUFFER_SIZE, 0 ) <= 0 )
		return ( -1 );
	readed = buffer;
	return ( 1 );
}

int	Receptionist::addNewClient(socket_t serverFd)
{
	socket_t	clientFd;
	struct pollfd	*clientPoll;
	
	clientFd = Sockets::acceptConnection( serverFd );
	if ( clientFd < 0 )
		return ( -1 );
	if ( polls.addPollfd( clientFd, POLLIN, 0, CPOLLFD ) == false )
	{
		Log::Error( "Too many clients trying to connect to server" );
		close( clientFd );
	}
	try
	{
		clientPoll = &polls[ clientFd ];
	}
	catch ( std::out_of_range& e ) { 
		Log::Error( "Failed to insert clientPoll" );
		close( clientFd );
		return ( -1 );
	}
	if (!newClient(clientFd, polls))
	{
		Log::Error( "Failed to append Request" );
		close( clientFd );
		return ( -1 );
	}
	return ( 1 );
}

void	Receptionist::manageClient(socket_t clientFd)
{
	struct pollfd	*clientPoll;
	std::string		readed;

	try
	{
		clientPoll = &(polls[ clientFd ]);
	}
	catch ( std::out_of_range& e )
	{ 
		Log::Info( "ClientPoll for [ " \
			+ SUtils::longToString( (long)clientFd )\
			+ " ]: " \
			+ "not found");
		return ;
	}
	Client * cli = operator[](clientFd);
	if (cli)
	{
		if ( clientPoll->revents & POLLIN )
		{
			if ( readRequest( clientFd, readed ) < 0 )
			{
				// Read Failed
				polls.closePoll( clientFd );
				return ;
			}
			Log::Info( "Readed [ " \
					+ SUtils::longToString( clientFd )\
					+ " ]: " \
					+ readed);
			cli->manageRecv(readed);
			if (cli->manageCompleteRecv())
				cli->allowPollWrite(true);
				// clientPoll->events |= POLLOUT;
		}
		else if ( clientPoll->revents & POLLOUT )
		{
			cli->managePollout();
			cli->allowPollWrite(false);
			// clientPoll->events &= ~POLLOUT;
			if ((cli->size() == 0 && cli->getPendingSize() ==0))
			{
			 	polls.closePoll( clientFd );
				eraseClient(clientFd);
			}
		}
	}
	else
	{
		Log::Info( "Client for [ " \
			+ SUtils::longToString( (long)clientPoll )\
			+ " ]: " \
			+ "not found");
	}
}

int	Receptionist::mainLoop(void)
{
	socket_t		serverFd;
	socket_t		clientFd;
	int				waitRes = 1;

	while ( true )
	{
		if ( WSSignals::isSig == true )
			return ( 1 );
		if (waitRes != 0)
			Log::Info( "Waiting for any fd ready to I/O" );
		waitRes = polls.wait( timeout );
		if ( waitRes < 0 )
			return ( 1 );
		CgiExecutor::attendPendingCgiTasks();
		// if (checkPendingToSend())
		// 	Log::Info("Some Pending To Send");
		// else
		// 	Log::Info("None Pending To Send");
		if ( waitRes == 0 )
		{
			// Log::Info( "Timeout Waiting for any fd ready to I/O" );
			continue;
		}
		serverFd = polls.isNewClient();
		if ( serverFd > 0 )
		{
			if ( addNewClient( serverFd ) < 0 )
				return ( 1 );
		}
		else
		{
			clientFd = polls.getPerformClient();
			if ( clientFd > 0 )
				manageClient( clientFd );
		}
	}
	return (0);
}

