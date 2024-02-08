/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 12:14:50 by omoreno-         ###   ########.fr       */
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
	this->servers = NULL;
	this->res = NULL;
	this->requestBodyRemain = 0;
	this->responseBodyRemain = 0;
	this->receptionist = NULL;
	this->responseHeaderSent = false;
	this->responseSent = false;
	this->resetCgiOperation();
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
	this->evs = bEvs;
	this->servers = servers;
	this->addr = info;
	this->res = NULL;
	this->requestBodyRemain = 0;
	this->responseBodyRemain = 0;
	this->receptionist = recp;
	this->responseHeaderSent = false;
	this->responseSent = false;
	this->resetCgiOperation();
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
	this->requestBodyRemain = b.requestBodyRemain;
	this->responseBodyRemain = b.responseBodyRemain;
	this->receptionist = b.receptionist;
	this->responseHeaderSent = b.responseHeaderSent;
	this->responseSent = b.responseSent;
	this->cgiDriven = b.cgiDriven;
	this->cgiTimeout = b.cgiTimeout;
	this->cgiFinished = b.cgiFinished;
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
		this->requestBodyRemain = b.requestBodyRemain;
		this->responseBodyRemain = b.responseBodyRemain;
		this->receptionist = b.receptionist;
		this->responseHeaderSent = b.responseHeaderSent;
		this->responseSent = b.responseSent;
		this->cgiDriven = b.cgiDriven;
		this->cgiTimeout = b.cgiTimeout;
		this->cgiFinished = b.cgiFinished;
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

const std::string&	Client::getCgiOutput( void ) const
{
	return ( this->cgiOutput );
}

void	Client::LogId( void ) const
{
	Log::Info( "Client id: " + SUtils::longToString( id ) );
}

Request	*Client::findRecvRequest( void )
{
	Request				*req = NULL;

	req = this->getPending();
	if ( req && req->isReceiving() )
		return ( req );
	return ( NULL );
}

Request	*Client::findCompleteRecvRequest( void )
{
	Request				*req = NULL;

	req = this->getPending();
	if ( req && req->isCompleteRecv() )
		return ( req );
	return ( NULL );
}

Request	*Client::findReadyToSendRequest( void )
{
	Request				*req;

	req = this->getPending();
	if ( req && req->isReadyToSend() )
		return ( req );
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
			req = appendRequest( this );
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

// int	Client::managePollout( void )
// {
// 	Request		*req = NULL;
// 	int			resSendStatus = Client::SENDING;

// 	if ( this->res )
// 		resSendStatus = sendResponse( this->res );		
// 	else if ( ( req = findReadyToSendRequest() ) )
// 	{
// 		this->res = Router::getResponse( req );
// 		if ( this->res )
// 		{
// 			this->res->updateResString();
// 			resSendStatus = sendResponse( this->res );
// 		}
// 		Requests::eraseRequest();
// 	}
// 	if ( resSendStatus == Client::SENT )
// 		Log::Success( "Response sent [ " + SUtils::longToString( this->socket ) + " ]" );
// 	return ( resSendStatus );
// }

bool	Client::getKeepAlive( void ) const
{
	return ( this->keepAlive );
}

// int	Client::sendResponse( Response *res )
// {
// 	int	resSendStatus = Client::ERROR;

// 	if ( this->socket >= 0 )
// 	{
// 		resSendStatus = Receptionist::sendResponse( this->socket, res );
// 		if ( resSendStatus == Client::ERROR || resSendStatus == Client::SENT )
// 		{
// 			delete this->res;
// 			this->res = NULL;
// 			// allowPollWrite( false );
// 		}
// 	}
// 	return ( resSendStatus );
// }

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

bool	Client::checkPendingToSend( void )
{
	LogId();
	if ( Requests::checkPendingToSend())
	{
		if (this->evs)
			this->evs->setEventWrite(this, this->socket);
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
	this->cgiDriven = true;
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

int	Client::deleteEventProcExit( pid_t pid )
{
	if ( this->evs )
		return ( this->evs->deleteEventProcExit(this, pid));
	return ( 0 );
}

int	Client::deleteEventProcTimeout( pid_t pid )
{
	if ( this->evs )
		return ( this->evs->deleteEventProcTimeout(this, pid));
	return ( 0 );
}

int	Client::onEventProcExit( Event& tevent )
{

	this->deleteEventProcTimeout(tevent.ident);
	this->cgiTimeout = false;
	this->cgiFinished = true;
	this->performCgiCompletion();
	return (0);
}

int	Client::onEventProcTimeout( Event& tevent )
{
	this->deleteEventProcExit(tevent.ident);
	kill(tevent.ident, SIGKILL);
	//TODO setError to Request, Response shoud update in accordance
	this->cgiTimeout = true;
	this->cgiFinished = true;
	this->performCgiCompletion();
	return (0);
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
	amount = readRequest( tevent.ident, readed );
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
	if (this->res)
		this->res->setBody( this->res->getBody() + content );
	if(amountToRead - actualRead == 0)
	{
		this->res->updateHeadersString();
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
	}
	return ( 0 );
}

int	Client::onEventReadPipe( Event& tevent )
{
	char	buffer[ BUFFER_SIZE ];
	size_t	amountToRead = BUFFER_SIZE;
	size_t	actualRead;

	createNewResponse();
	if (amountToRead > (size_t)tevent.data)
		amountToRead = (size_t)tevent.data;
	actualRead = read(tevent.ident, buffer, amountToRead);
	std::string content(buffer, actualRead);
	this->cgiOutput += content;
	if (tevent.flags & EV_EOF)
	{
		if (this->res && this->front())
		{
			Router::updateResponse( *this->res, *this->front(), *this );
			this->res->updateHeadersString();
		}
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
		this->performCgiCompletion();
	}
	return ( 0 );
}

int	Client::onEventWriteSocket( Event& tevent )
{
	int			resSendStatus;

	(void)tevent;
	if ( this->res )
	{
		resSendStatus = this->res->sendResponse( this->socket );
		if ( resSendStatus == Response::ERROR || resSendStatus == Response::SENT )
		{
			delete this->res;
			this->res = NULL;
			if ( this->front()->isReadyToSend() )
				this->eraseRequest();
			//TODO See if is time to remove the client
				
		}
	}
	return ( 0 );
}

int	Client::onEventWriteFile( Event& tevent )
{
	Request* req = this->getPending();
	if (!req)
	{
		this->writeEOF = true;
		return 0;
	}
	if ((size_t)tevent.data > 0)
	{
		ssize_t actualWr = write(tevent.ident, req->getBody().c_str(), BUFFER_SIZE );
		if (actualWr > 0)
			req->eraseBody(actualWr);
	}
	if (req->isCompleteRecv() && req->getBodyLength() > 0)
	{
		close(tevent.ident);
		this->writeEOF = true;
	}
	return 0;
}

int	Client::onEventWritePipe( Event& tevent )
{
	Request* req = this->getPending();
	if (!req)
	{
		this->writeEOF = true;
		return 0;
	}
	if ((size_t)tevent.data > 0)
	{
		ssize_t actualWr = write(tevent.ident, req->getBody().c_str(), BUFFER_SIZE );
		if (actualWr > 0)
			req->eraseBody(actualWr);
	}
	if (tevent.flags & EV_EOF)
	{
		close(tevent.ident);
		this->writeEOF = true;
		this->performCgiCompletion();
	}
	return 0;
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
	else if ( tevent.filter & EVFILT_PROC )
		return ( onEventProcExit( tevent ) );
	else if ( tevent.filter & EVFILT_TIMER )
		return ( onEventProcTimeout( tevent ) );
	return ( 0 );
}

int	Client::readRequest( socket_t clientFd, std::string& readed )
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

void	Client::setCompletedRequest( void )
{
	Request	*req = getPending();
	if (req)
		req->setCompletedRequest();
}

void	Client::setTimeoutedRequest( void )
{
	Request	*req = getPending();
	if (req)
		req->setTimeoutedRequest();
}

void	Client::performCgiCompletion( void )
{
	if (this->cgiDriven && this->cgiFinished && this->readEOF && this->writeEOF)
	{
		if (this->cgiTimeout)
			setTimeoutedRequest();
		else
			setCompletedRequest();
	}		
}

void	Client::resetCgiOperation( void )
{
	this->cgiDriven = false;
	this->cgiTimeout = false;
	this->cgiFinished = false;
	this->pipeCgiWrite = -1;
	this->pipeCgiRead = -1;
	this->cgiOutput = std::string("");
}