/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:44:28 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/05 11:03:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Receptionist.hpp>
#include <Response.hpp>

Receptionist::Receptionist( ServersVector& servers ): Clients(), \
													polls( MAX_CLIENTS ), \
													_servers( servers ), \
													timeout( 10 )
{
	socket_t				serverFd;
	Directives				*d = NULL;
	int						backlog = 10;
	ServersVector::iterator	itb = this->_servers.begin();
	ServersVector::iterator	it = itb;
	struct sockaddr_in		info;

	while ( it != this->_servers.end() )
	{
		d = it->getDirectives();
		if ( d != NULL && d->isEmpty() == false )
		{
			if ( serverShareAddr( itb, it, info ) == false )
			{
				serverFd = Sockets::createPassiveSocket( d->getHost(), \
								d->getPort(), backlog, info );
				this->polls.addPollfd( serverFd, POLLIN, 0, SPOLLFD );
			}
			it->setAddr( info );
			it++;
		}
		else
			this->_servers.erase( it );
	}
}

Receptionist::~Receptionist( void )
{
	Log::Error( "Calling Receptionist destructor" );
}

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

bool	Receptionist::serverShareAddr( ServersVector::iterator& begin, \
										ServersVector::iterator& curr, \
										struct sockaddr_in& info )
{
	struct sockaddr_in	addr;
	unsigned int		ip;

	addr = Sockets::codeHostToAddrin( curr->getHost().c_str(), curr->getPort() );
	ip = addr.sin_addr.s_addr;
	for ( ServersVector::iterator it = begin; it != curr; it++ )
	{
		if ( it->getPort() == curr->getPort() && it->getIpNetworkOrder() == ip )
		{
			info = it->getAddr();
			return ( true );
		}
	}
	return ( false );
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
	return ( 1 );
}

int	Receptionist::readRequest( socket_t clientFd, std::string& readed )
{
	char	buffer[ BUFFER_SIZE + 1 ];
	ssize_t	amount;

	amount = recv( clientFd, buffer, BUFFER_SIZE, 0 );
	if ( amount < 0 )
		return ( -1 );
	readed += std::string(buffer, amount);
	return ( amount );
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
	int			amount;

	amount = readRequest( clientFd, readed );
	if ( amount < 0 || (amount == 0 && CgiExecutor::findClientPendingPid(cli) == 0))
	{
		// Read Failed or finish to read and not pending of timeout
		polls.closePoll( clientFd );
		eraseClient( cli );
		return ;
	}
	Log::Info( "Readed " \
			+ SUtils::longToString( amount ) \
			+ " bytes from [ " \
			+ SUtils::longToString( clientFd ) \
			+ " ]: " \
			+ readed );
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
		//if ( waitRes != 0 )
			//Log::Info( "Waiting for any fd ready to I/O" );
		waitRes = polls.wait( timeout );
		if ( waitRes < 0 )
			return ( 1 );
		CgiExecutor::attendPendingCgiTasks();
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
