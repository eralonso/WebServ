/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:44:28 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/07 17:34:09 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Receptionist.hpp>
#include <Response.hpp>

Receptionist::Receptionist( ServersVector& servers ): Clients(), \
													_servers( servers ), \
													timeout( 1 )
{
	if ( evs.isCreate() == false )
		throw std::runtime_error( "Cant't create a kqueue" );
	setupServers();
}

Receptionist::~Receptionist( void )
{
}

Receptionist::Receptionist( const Receptionist& b ): Clients(), \
													_servers( b._servers ), \
													timeout( b.timeout ) {}

Receptionist& 	Receptionist::operator=( const Receptionist& b )
{
	if ( this != &b )
	{
		this->_servers = b._servers;
		this->timeout = b.timeout;
	}
	return ( *this );
}

void	Receptionist::setupServers( void )
{
	socket_t				serverFd;
	Directives				*d = NULL;
	ServersVector::iterator	itb = this->_servers.begin();
	ServersVector::iterator	it = itb;
	struct sockaddr_in		info;

	while ( it != this->_servers.end() )
	{
		d = it->getDirectives();
		if ( d != NULL && d->isEmpty() == false )
		{
			if ( serverShareAddr( itb, it, info, serverFd ) == false )
			{
				serverFd = Sockets::createPassiveSocket( d->getHost(), \
								d->getPort(), BACKLOG, info );
			}
			it->setAddr( info );
			it->setSocketFd( serverFd );
			it->setReceptionist( this );
			it->setEvents( &this->evs );
			it->setEventRead();
			it++;
		}
		else
			this->_servers.erase( it );
	}
}

bool	Receptionist::serverShareAddr( ServersVector::iterator& begin, \
										ServersVector::iterator& curr, \
										struct sockaddr_in& info, \
	   									socket_t& serverFd )
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
			serverFd = it->getSocketFd();
			return ( true );
		}
	}
	return ( false );
}

const ServersVector&	Receptionist::getServers( void ) const
{
	return ( this->_servers );
}

// int	Receptionist::sendResponse( socket_t socket, Response *res )
// {
// 	size_t		threshold;
// 	const char	*str;
// 	size_t		size;
// 	int			ret;
// 	size_t		pos;

// 	pos = res->getSendPos();
// 	size = res->getResString().size() - pos; 
// 	str = res->getResString().c_str();
// 	threshold = size > SEND_BUFFER_SIZE ? SEND_BUFFER_SIZE : size;
// 	if ( ( ret = send( socket, str + pos, threshold, MSG_DONTWAIT ) < 0 ) )
// 	{
// 		Log::Error( "Failed to send response with code: " + SUtils::longToString( ret ) );
// 		return ( Client::ERROR );
// 	}
// 	// Log::Info( "SendResponse[" + res->getResString() + "]" );
// 	pos = res->increaseSendPos( threshold );
// 	if ( pos >= res->getResString().size() )
// 		return ( Client::SENT );
// 	return ( Client::SENDING );
// }

int	Receptionist::readRequest( socket_t clientFd, std::string& readed )
{
	char	buffer[ BUFFER_SIZE + 1 ];
	ssize_t	amount;

	amount = recv( clientFd, buffer, BUFFER_SIZE, MSG_DONTWAIT );
	Log::Info( "amount: " + SUtils::longToString( amount ) );
	Log::Info( "recv errno: " + SUtils::longToString( errno ) );
	if ( amount < 0 )
		return ( -1 );
	readed += std::string(buffer, amount);
	return ( amount );
}

// int	Receptionist::addNewClient( socket_t serverFd )
// {
// 	socket_t			clientFd;
// 	struct sockaddr_in	info;
	
// 	clientFd = Sockets::acceptConnection( serverFd, info );
// 	if ( clientFd < 0 )
// 		return ( -1 );
// 	if ( !Clients::newClient( clientFd, &this->evs, _servers, info ) )
// 	{
// 		Log::Error( "Failed to append Request" );
// 		close( clientFd );
// 		return ( -1 );
// 	}
// 	return ( 1 );
// }

//int	Receptionist::addNewClient( socket_t serverFd )
//{
//	socket_t			clientFd;
//	struct sockaddr_in	info;
//	
//	clientFd = Sockets::acceptConnection( serverFd, info );
//	if ( clientFd < 0 )
//		return ( -1 );
//	if ( polls.addPollfd( clientFd, POLLIN, 0, CPOLLFD ) == false )
//	{
//		Log::Error( "Too many clients trying to connect to server" );
//		close( clientFd );
//	}
//	else if ( !Clients::newClient( clientFd, polls, _servers, info ) )
//	{
//		Log::Error( "Failed to append Request" );
//		close( clientFd );
//		return ( -1 );
//	}
//	return ( 1 );
//}

// void	Receptionist::manageClientRead( socket_t clientFd, Client *cli )
// {
// 	std::string	readed;
// 	int			amount;

// 	amount = readRequest( clientFd, readed );
// 	//if ( amount < 0 || ( amount == 0 
// 	//	&& CgiExecutor::findClientPendingPid( cli ) == 0 ) )
// 	if ( amount < 0 )
// 	{
// 		// Read Failed or finish to read and not pending of timeout
// 		cli->closeSocket();
// 		eraseClient( cli );
// 		return ;
// 	}
// 	// Log::Info( "Readed " 
// 	// 		+ SUtils::longToString( amount ) 
// 	// 		+ " bytes from [ " 
// 	// 		+ SUtils::longToString( clientFd ) 
// 	// 		+ " ]: =>\n" 
// 	// 		+  readed );
// 			// + SUtils::compactString(readed, 200, 80, 40 ) );
// 	cli->manageRecv( readed );
// 	if ( cli->manageCompleteRecv() )
// 		cli->allowPollWrite( true );
// }

// void	Receptionist::manageClientWrite( socket_t clientFd, Client *cli )
// {
// 	// if ( cli->size() == 0 && cli->getPendingSize() == 0 && !cli->getKeepAlive() )
// 	if ( !cli->managePollout() || ( cli->size() == 0 && cli->getPendingSize() == 0 \
// 		&& !cli->isResponsePendingToSend() && !cli->getKeepAlive() ) )
// 	{
// 		polls.closePoll( clientFd );
// 		eraseClient( clientFd );
// 	}
// }

//void	Receptionist::manageClient( socket_t clientFd )
//{
//	struct pollfd	*clientPoll = NULL;
//	Client			*cli = NULL;
//
//	try
//	{
//		clientPoll = &( polls[ clientFd ] );
//		cli = this->at( clientFd );
//	}
//	catch ( std::out_of_range& e )
//	{ 
//		Log::Error( "Client for [ " \
//			+ SUtils::longToString( clientFd ) \
//			+ " ]: not found");
//		return ;
//	}
//	if ( clientPoll->revents & POLLOUT )
//		manageClientWrite( clientFd, cli );
//	else if ( clientPoll->revents & POLLIN )
//		manageClientRead( clientFd, cli );
//}

int	Receptionist::mainLoop( void )
{
	return ( this->evs.loopEvents() );
}

//int	Receptionist::mainLoop( void )
//{
//	socket_t	serverFd;
//	socket_t	clientFd;
//	int			waitRes = 1;
//
//	while ( WSSignals::isSig == false )
//	{
//		waitRes = polls.wait( timeout );
//		if ( waitRes < 0 )
//			return ( 1 );
//		//CgiExecutor::attendPendingCgiTasks();
//		if ( waitRes == 0 )
//			continue ;
//		Log::Info( "VUELTITA" );
//		serverFd = polls.isNewClient();
//		if ( serverFd > 0 )
//			addNewClient( serverFd );
//		else
//		{
//			clientFd = polls.getPerformClient();
//			if ( clientFd > 0 )
//				manageClient( clientFd );
//		}
//	}
//	return ( WSSignals::isSig );
//}
