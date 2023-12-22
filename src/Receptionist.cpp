/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:44:28 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/22 12:14:23 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Receptionist.hpp>
#include <Response.hpp>

Receptionist::Receptionist( ServersVector& servers ): Clients(), \
													polls( MAX_CLIENTS ), \
													_servers( servers ), \
													timeout( 50 )
{
	socket_t				serverFd;
	Directives				*d;
	int						backlog = 10;
	ServersVector::iterator	it;
	struct sockaddr_in		info;

	it = this->_servers.begin();
	while ( it != this->_servers.end() )
	{
		d = it->getDirectives();
		if ( d != NULL && d->isEmpty() == false )
		{
			serverFd = Sockets::createPassiveSocket( d->getHost(), \
							d->getPort(), backlog, info );
			it->setAddr( info );
			this->polls.addPollfd( serverFd, POLLIN, 0, SPOLLFD );
			it++;
		}
		else
			this->_servers.erase( it );
	}
}

Receptionist::~Receptionist( void ) {}

Receptionist::Receptionist( const Receptionist& b ): Clients(), \
													polls( b.polls ), \
													_servers( b._servers ), \
													timeout( b.timeout ) {}

Receptionist& 	Receptionist::operator=( const Receptionist& b )
{
	if ( this != &b )
	{
		this->polls = b.polls;
		this->_servers = b._servers;
		this->timeout = b.timeout;
	}
	return ( *this );
}

int	Receptionist::sendResponse( socket_t connected, std::string response )
{
	if ( send( connected, response.c_str(), response.size(), 0 ) < 0 )
	{
		Log::Error( "Failed to send response" );
		throw std::logic_error( "Failed to send response" );
	}
	Log::Success( "Response sended [ " \
			+ SUtils::longToString( connected ) \
			+ " ]" );
	// Log::Success( response );
	return ( 1 );
}

int	Receptionist::readRequest( socket_t clientFd, std::string& readed )
{
	char	buffer[ BUFFER_SIZE + 1 ];

	ssize_t amount;
	ssize_t totalAmount = 0;
	while(true)
	{
		memset( buffer, 0, BUFFER_SIZE + 1 );
		amount = recv( clientFd, buffer, BUFFER_SIZE, 0);
		Log::Info("Received " + SUtils::longToString(amount) + " bytes");
		totalAmount += amount;
		if ( amount < 0 )
			return ( -1 );
		readed += std::string(buffer, amount);
		if (amount < BUFFER_SIZE)
		{
			if (totalAmount == 0)
				return ( -1 );
			return ( 1 );
		}
	}
}

int	Receptionist::addNewClient( socket_t serverFd )
{
	socket_t			clientFd;
	struct sockaddr_in	info;
	
	clientFd = Sockets::acceptConnection( serverFd, info );
	if ( clientFd < 0 )
		return ( -1 );
	if ( polls.addPollfd( clientFd, POLLIN, 0, CPOLLFD ) == false )
	{
		Log::Error( "Too many clients trying to connect to server" );
		close( clientFd );
	}
	else if ( !Clients::newClient( clientFd, polls, _servers, info ) )
	{
		Log::Error( "Failed to append Request" );
		close( clientFd );
		return ( -1 );
	}
	return ( 1 );
}

void	Receptionist::manageClientRead( socket_t clientFd, Client *cli )
{
	std::string	readed;

	if ( readRequest( clientFd, readed ) < 0 )
	{
		// Read Failed
		polls.closePoll( clientFd );
		return ;
	}
	Log::Info( "Readed [ " \
			+ SUtils::longToString( clientFd ) \
			+ " ]: " \
			+ readed);
	cli->manageRecv( readed );
	if ( cli->manageCompleteRecv() )
		cli->allowPollWrite( true );
}

void	Receptionist::manageClientWrite( socket_t clientFd, Client *cli )
{
	cli->managePollout();
	cli->allowPollWrite( false );
	if ( cli->size() == 0 && cli->getPendingSize() == 0 )
	{
		polls.closePoll( clientFd );
		eraseClient( clientFd );
	}
}

void	Receptionist::manageClient( socket_t clientFd )
{
	struct pollfd	*clientPoll = NULL;
	Client			*cli = NULL;

	try
	{
		clientPoll = &( polls[ clientFd ] );
		cli = this->at( clientFd );
	}
	catch ( std::out_of_range& e )
	{ 
		Log::Error( "Client for [ " \
			+ SUtils::longToString( clientFd ) \
			+ " ]: not found");
		return ;
	}
	if ( clientPoll->revents & POLLIN )
		manageClientRead( clientFd, cli );
	else if ( clientPoll->revents & POLLOUT )
		manageClientWrite( clientFd, cli );
}

int	Receptionist::mainLoop( void )
{
	socket_t	serverFd;
	socket_t	clientFd;
	int			waitRes = 1;

	while ( WSSignals::isSig == false )
	{
		if ( waitRes != 0 )
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
			continue ;
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
	return ( WSSignals::isSig );
}
