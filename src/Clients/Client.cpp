/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/07 11:52:27 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Response.hpp"
#include "Receptionist.hpp"
#include "Router.hpp"

size_t	Client::id_counter = 0;

Client::Client( void ): EventsTarget( NULL )
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->keepAlive = false;
	this->pending = 0;
	this->socket = -1;
	this->fileFd = -1;
	this->pipeCgiWrite = -1;
	this->pipeCgiRead = -1;
	this->servers = NULL;
	this->res = NULL;
	this->receptionist = NULL;
	this->responseSent = false;
	SUtils::memset( &this->addr, 0, sizeof( this->addr ) );
}

Client::Client( socket_t socket, Events *bEvs, const ServersVector *servers, \
	   				struct sockaddr_in& info, Receptionist *recp ): EventsTarget( bEvs )
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->keepAlive = false;
	this->pending = 0;
	this->socket = socket;
	this->fileFd = -1;
	this->pipeCgiWrite = -1;
	this->pipeCgiRead = -1;
	this->evs = bEvs;
	this->servers = servers;
	this->addr = info;
	this->res = NULL;
	this->receptionist = recp;
	this->responseSent = false;
}

Client::~Client( void )
{
	if ( this->res != NULL )
		delete this->res;
	close( this->socket );
	close( this->fileFd );
	close( this->pipeCgiRead );
	close( this->pipeCgiWrite );
}

Client::Client( const Client& b ): Requests(), EventsTarget( b.evs )
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->socket = b.socket;
	this->fileFd = b.fileFd;
	this->pipeCgiWrite = b.pipeCgiWrite;
	this->pipeCgiRead = b.pipeCgiRead;
	this->pending = b.pending;
	this->received = b.received;
	this->servers = b.servers;
	this->addr = b.addr;
	this->res = b.res;
	this->receptionist = b.receptionist;
	this->responseSent = b.responseSent;
}

Client&	Client::operator=( const Client& b )
{
	if ( this != &b )
	{
		this->socket = b.socket;
		this->fileFd = b.fileFd;
		this->pipeCgiWrite = b.pipeCgiWrite;
		this->pipeCgiRead = b.pipeCgiRead;
		this->pending = b.pending;
		this->received = b.received;
		this->servers = b.servers;
		this->addr = b.addr;
		this->res = b.res;
		this->receptionist = b.receptionist;
		this->responseSent = b.responseSent;
	}
	return ( *this );
}

int Client::bindClientPoll( socket_t socket )
{
	this->socket = socket;
	return ( 0 );
}

void	Client::createNewResponse( void )
{
	if ( !this->res )
		this->res = new Response;
}

bool	Client::isResponseSent( void ) const
{
	return ( this->responseSent );
}

socket_t	Client::getClientSocket( void ) const
{
	return ( this->socket );
}

size_t	Client::getId( void ) const
{
	return ( this->id );
}

const struct sockaddr_in&	Client::getAddr( void ) const
{
	return ( this->addr );
}

std::string	Client::getIpString( void ) const
{
	return ( Binary::decodeAddress( ntohl( addr.sin_addr.s_addr ) ) );
}

unsigned int	Client::getIpNetworkOrder( void ) const
{
	return ( addr.sin_addr.s_addr );
}

unsigned int	Client::getIpHostOrder( void ) const
{
	return ( ntohl( addr.sin_addr.s_addr ) );
}

const ServersVector&	Client::getServers( void ) const
{
	return ( *this->servers );
}

void	Client::LogId( void ) const
{
	Log::Info( "Client id: " + SUtils::longToString( id ) );
}

Request	*Client::findRecvRequest( void )
{
	Request				*req = NULL;
	Requests::iterator	it = this->begin();
	Requests::iterator	ite = this->end();

	while ( it != ite )
	{
		req = *it;
		if ( req && req->isReceiving() )
			return ( req );
		it++;
	}
	return ( NULL );
}

Request	*Client::findCompleteRecvRequest( void )
{
	Request				*req = NULL;
	Requests::iterator	it = this->begin();
	Requests::iterator	ite = this->end();

	while ( it != ite )
	{
		ite--;
		req = *ite;
		if ( req && req->isCompleteRecv() )
			return ( req );
	}
	return ( NULL );
}

Request	*Client::findReadyToSendRequest( void )
{
	Request				*req;
	Requests::iterator	ite = this->end();

	if ( this->size() > 0 )
	{
		ite--;
		req = *ite;
		if ( req && req->isReadyToSend() )
			return ( req );
	}
	return ( NULL );
}

int	Client::manageRecv( std::string recv )
{
	std::string	line;
	bool		fail = false;
	bool		cont = true;
	Request		*req = NULL;

	this->received += recv;
	while ( cont && !fail && getPendingSize() > 0 )
	{
		req = findRecvRequest();
		if ( req == NULL )
		{
			req = Requests::appendRequest( this );
			if ( req == NULL )
				fail = true;
		}
		if ( !fail )
		{
			cont = req->processRecv();
			if ( req->isCompleteRecv() )
				cont = false;
		}
	}
	purgeUsedRecv();
	if ( fail == true )
	{
		Log::Error( "Coudn't create new Request" );
		return ( 1 );
	}
	return ( 0 );
}

int	Client::manageCompleteRecv( void )
{
	Request		*req = NULL;
	int			count = 0;

	while ( ( req = findCompleteRecvRequest() ) )
	{
		if ( Router::processRequestReceived( *req ) )
			count++;
	}
	return ( count );
}

int	Client::managePollout( void )
{
	Request		*req = NULL;
	int			resSendStatus = Client::SENDING;

	if ( this->res )
		resSendStatus = sendResponse( this->res );		
	else if ( ( req = findReadyToSendRequest() ) )
	{
		this->res = Router::getResponse( req );
		if ( this->res )
		{
			this->res->updateResString();
			resSendStatus = sendResponse( this->res );
		}
		Requests::eraseRequest();
	}
	if ( resSendStatus == Client::SENT )
		Log::Success( "Response sent [ " + SUtils::longToString( this->socket ) + " ]" );
	return ( resSendStatus );
}

bool	Client::getKeepAlive( void ) const
{
	return ( this->keepAlive );
}

int	Client::sendResponse( Response *res )
{
	int	resSendStatus = Client::ERROR;

	if ( this->socket >= 0 )
	{
		resSendStatus = Receptionist::sendResponse( this->socket, res );
		if ( resSendStatus == Client::ERROR || resSendStatus == Client::SENT )
		{
			delete this->res;
			this->res = NULL;
			// allowPollWrite( false );
		}
	}
	return ( resSendStatus );
}

bool	Client::getLine( std::string& line )
{
	size_t	found = this->received.find( '\n', this->pending );

	if ( found == std::string::npos )
		return ( false );
	line = this->received.substr( this->pending, found - this->pending );
	this->pending = found + 1;
	return ( true );
}

size_t	Client::getNChars( std::string &data, size_t n )
{
	size_t	remain = this->received.size() - this->pending;

	if ( n > remain )
		n = remain;
	data = this->received.substr( this->pending, n );
	this->pending += n;
	return ( n );
}

size_t	Client::getPendingSize( void ) const
{
	return ( this->received.size() - this->pending );
}

int	Client::getFileFd( void ) const
{
	return ( this->fileFd );
}

int	Client::getPipeCgiWrite( void ) const
{
	return ( this->pipeCgiWrite );
}

int	Client::getPipeCgiRead( void ) const
{
	return ( this->pipeCgiRead );
}

socket_t	Client::getSocket( void ) const
{
	return ( this->socket );
}

bool	Client::setKeepAlive( bool value )
{
	this->keepAlive = value;
	
	return ( this->keepAlive );
}

size_t	Client::purgeUsedRecv( void )
{
	size_t	pendSize = getPendingSize();

	this->received = this->received.substr( this->pending, pendSize );
	this->pending = 0;
	return ( pendSize );
}

// void	Client::allowPollWrite( bool value )
// {
// 	struct pollfd	*clientPoll = NULL;

// 	if ( this->polls != NULL )
// 	{
// 		try
// 		{
// 			clientPoll = &( polls->operator[]( socket ) );
// 		}
// 		catch ( std::out_of_range& e )
// 		{ 
// 			Log::Info( "ClientPoll for [ " \
// 				+ SUtils::longToString( this->socket ) \
// 				+ " ]: not found" );
// 			return ;
// 		}
// 		if ( value )
// 			clientPoll->events = POLLOUT;
// 		else 
// 			clientPoll->events = POLLIN;
// 	}
// 	else
// 		Log::Error( "Polls not found on Client " \
// 			+ SUtils::longToString( this->id ) );
// }

bool	Client::checkPendingToSend( void )
{
	LogId();
	if ( Requests::checkPendingToSend() )
	{
		// allowPollWrite( true );
		return ( true );
	}
	return ( false );
}

bool	Client::isResponsePendingToSend( void ) const
{
	return (this->res != NULL);
}

void	Client::setFileFd( int fd )
{
	this->fileFd = fd;
}

void	Client::setPipeCgiWrite( int fd )
{
	this->pipeCgiWrite = fd;
}

void	Client::setPipeCgiRead( int fd )
{
	this->pipeCgiRead = fd;
}

int	Client::setEventReadSocket( void )
{
	if ( this->evs )
		return ( this->evs->setEventRead( this, this->socket ) );
	return ( 0 );
}
int	Client::setEventWriteSocket( void )
{
	if ( this->evs )
		return ( this->evs->setEventWrite( this, this->socket ) );
	return ( 0 );
}

int	Client::setEventProc( int pipeRead, int pipeWrite, int pid )
{
	this->pipeCgiRead = pipeRead;
	this->pipeCgiWrite = pipeWrite;
	if ( this->evs )
	{
		if ( this->evs->setEventProcExit( this, pid, CGI_TO ) )
			return ( -1 );
		if ( this->evs->setEventRead( this, pipeRead ) )
			return ( -1 );
		if ( this->evs->setEventWrite( this, pipeWrite ) )
			return ( -1 );
	}
	return ( 0 );
}

int	Client::setEventReadFile( int fd )
{
	this->fileFd = fd;
	if ( this->evs )
		return ( this->evs->setEventRead( this, fd ) );
	return ( 0 );
}

int	Client::setEventWriteFile( int fd )
{
	this->fileFd = fd;
	if ( this->evs )
		return ( this->evs->setEventWrite( this, fd ) );
	return ( 0 );
}

int	Client::enableEventReadSocket( bool enable )
{
	if ( this->evs )
		return ( this->evs->enableEventRead( this, this->socket, enable ) );
	return ( 0 );
}

int	Client::enableEventWriteSocket( bool enable )
{
	if ( this->evs )
		return ( this->evs->enableEventWrite( this, this->socket, enable ) );
	return ( 0 );
}

int	Client::onEventReadSocket( Event& tevent )
{
	std::string	readed;
	int			amount;

	if ( tevent.flags & EV_EOF )
	{
		this->receptionist->eraseClient( this );
		return ( 0 );
	}
	amount = Receptionist::readRequest( tevent.ident, readed );
	if ( amount < 0 )
	{
		this->receptionist->eraseClient( this );
		return ( -1 );
	}
	manageRecv( readed );
	if ( manageCompleteRecv() && !isResponseSent() )
		enableEventReadSocket( false );
	return ( 0 );
}

int	Client::onEventReadFile( Event& tevent )
{
	char	buffer[ BUFFER_SIZE ];
	size_t	amountToRead = BUFFER_SIZE;
	size_t	actualRead;

	createNewResponse();
	if (amountToRead > (size_t)tevent.data)
		amountToRead = (size_t)tevent.data;
	actualRead = read(tevent.ident, buffer, amountToRead);
	std::string content(buffer, actualRead);
	this-> += content;
	printf("EVFILT_READ event called id: '%ld' filt: '%hd' data:'%ld' actual read: '%p'\n", 
		tevent.ident, tevent.filter, tevent.data, content.c_str());
	if (tevent.flags & EV_EOF)
		printf("READ reached EOF\n");
	return ( 1 );
}

int	Client::onEventReadPipe( Event& tevent )
{
	( void )tevent;
	return ( 1 );
}

int	Client::onEventWriteSocket( Event& tevent )
{
	( void )tevent;
	return ( 1 );
}

int	Client::onEventWriteFile( Event& tevent )
{
	( void )tevent;
	return ( 1 );
}

int	Client::onEventWritePipe( Event& tevent )
{
	( void )tevent;
	return ( 1 );
}

int	Client::onEventRead( Event& tevent )
{
	int	ident;

	ident = tevent.ident;
	if ( this->socket == ident )
		return ( onEventReadSocket( tevent ) );
	if ( this->fileFd == ident )
		return ( onEventReadFile( tevent ) );
	if ( this->pipeCgiRead == ident )
		return ( onEventReadPipe( tevent ) );
	return ( 0 );
}

int	Client::onEventWrite( Event& tevent )
{
	int	ident;

	ident = tevent.ident;
	if ( this->socket == ident )
		return ( onEventWriteSocket( tevent ) );
	if ( this->fileFd == ident )
		return ( onEventWriteFile( tevent ) );
	if ( this->pipeCgiWrite == ident )
		return ( onEventWritePipe( tevent ) );
	return ( 0 );
}

int	Client::onEvent( Event& tevent )
{
	if ( tevent.filter & EVFILT_READ )
		return ( onEventRead( tevent ) );
	else if ( tevent.filter & EVFILT_WRITE )
		return ( onEventWrite( tevent ) );
	return ( 0 );
}
