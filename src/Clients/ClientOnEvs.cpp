/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientOnEvs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:12:13 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 12:00:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Receptionist.hpp"

int	Client::onEventProcExit( Event& tevent )
{
	Log::Debug( "onEventProcExit [" + SUtils::longToString(tevent.ident) + "]" );
	this->deleteEventProcTimeout(tevent.ident);
	this->cgiTimeout = false;
	this->cgiFinished = true;
	this->performCgiCompletion();
	return (0);
}

int	Client::onEventProcTimeout( Event& tevent )
{
	Log::Debug( "onEventProcTimeout [" + SUtils::longToString(tevent.ident) + "]" );
	this->deleteEventProcExit(tevent.ident);
	kill(tevent.ident, SIGKILL);
	this->cgiTimeout = true;
	this->cgiFinished = true;
	this->performCgiCompletion();
	return (0);
}

int	Client::onEventReadSocket( Event& tevent )
{
	std::string	readed;
	int			amount;

	Log::Debug( "onEventReadSocket [" + SUtils::longToString(tevent.ident) + "]" );
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
	if ( findCompleteRecvRequest() && isResponseSent() )
	{
		this->eraseRequest();
		this->reset();
		enableEventReadSocket( true );
	}
	else if ( findCompleteRecvRequest() && !isResponseSent() )
		enableEventReadSocket( false );
	return ( 0 );
}

int	Client::onEventReadFile( Event& tevent )
{
	char	buffer[ BUFFER_SIZE ];
	size_t	amountToRead = BUFFER_SIZE;
	size_t	actualRead;

	Log::Debug( "onEventReadFile [" + SUtils::longToString(tevent.ident) + "]" );
	createResponse();
	if (amountToRead > (size_t)tevent.data)
		amountToRead = (size_t)tevent.data;
	actualRead = read(tevent.ident, buffer, amountToRead);
	Log::Debug( "actualRead: " + SUtils::longToString( actualRead ) );
	Log::Debug( "data: " + SUtils::longToString( tevent.data ) );
	std::string content(buffer, actualRead);
	if (this->res)
		this->res->setBody( this->res->getBody() + content );
	if(tevent.data == (intptr_t)actualRead)
	{
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
		this->fileFd = -1;
	}
	return ( 0 );
}

int	Client::onEventWriteFile( Event& tevent )
{
	size_t	size;
	Request	*req = this->getPending();

	Log::Debug( "onEventWriteFile [" + SUtils::longToString(tevent.ident) + "]" );
	if (!req)
	{
		this->writeEOF = true;
		return 0;
	}
	if ((size_t)tevent.data > 0)
	{
		size = req->getBodyLength();
		if ( size > BUFFER_SIZE )
			size = BUFFER_SIZE;
		ssize_t actualWr = write( tevent.ident, req->getBody().c_str(), size );
		if ( actualWr < 0 )
			Log::Error( "can't write on file" );
		else if ( actualWr > 0)
			req->eraseBody( actualWr );
		Log::Debug( "amount of bytes wrote [ " + SUtils::longToString( actualWr ) + " ]" );
	}
	if (req->isCompleteRecv() && req->getBodyLength() == 0)
	{
		this->writeEOF = true;
		setEventWriteSocket();
		close(tevent.ident);
		this->fileFd = -1;
	}
	req->logStatus();
	Log::Debug( "req->getBodyLength() [ " + SUtils::longToString( req->getBodyLength() ) + " ]" );
	return 0;
}

int	Client::onEventReadPipe( Event& tevent )
{
	char	buffer[ BUFFER_SIZE ];
	size_t	amountToRead = BUFFER_SIZE;
	size_t	actualRead;

	Log::Debug( "onEventReadPipe [" + SUtils::longToString(tevent.ident) + "]" );
	createResponse();
	if (amountToRead > (size_t)tevent.data)
		amountToRead = (size_t)tevent.data;
	actualRead = read(tevent.ident, buffer, amountToRead);
	std::string content(buffer, actualRead);
	this->cgiOutput += content;
	if (! this->front()->isDocumentNPH() && this->cgiHeaderReached == -1)
	{
		this->cgiHeaderReached = this->CgiFindHeaderReached();
		if (this->cgiHeaderReached != -1)
			this->cgiContentLength = this->CgiFindContentLength();
	}
	if (tevent.flags & EV_EOF || isCgiContentLengthOverpass())
	{
		adjustCgiContentLengthOverpass();
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
		this->pipeCgiRead = -1;
		this->performCgiCompletion();
	}
	return ( 0 );
}


int	Client::onEventWritePipe( Event& tevent )
{
	size_t	size;	

	Request* req = this->getPending();
	if (!req)
	{
		this->writeEOF = true;
		return 0;
	}
	if ((size_t)tevent.data > 0)
	{
		size = req->getBodyLength();
		if ( size > BUFFER_SIZE )
			size = BUFFER_SIZE;
		ssize_t actualWr = write(tevent.ident, req->getBody().c_str(), size );
		if (actualWr > 0)
			req->eraseBody(actualWr);
	}
	if ( tevent.flags & EV_EOF || ( req->isCompleteRecv() && req->getBodyLength() == 0 ) )
	{
		this->writeEOF = true;
		this->performCgiCompletion();
		close(tevent.ident);
		this->pipeCgiWrite = -1;
	}
	req->logStatus();
	return 0;
}

int	Client::onEventWriteSocket( Event& tevent )
{
	int			resSendStatus;

	Log::Debug( "onEventWriteSocket [" + SUtils::longToString(tevent.ident) + "]");
	createResponse();
	if ( this->res )
	{
		if ( this->res->getSendStatus() == Response::GETTING_DATA )
			Router::updateResponse( *this->res, *this->front(), *this );
		resSendStatus = this->res->sendResponse( tevent );
		if ( resSendStatus == Response::ERROR)
		{
			Log::Debug( "onEventWriteSocket was Response::ERROR" );
			this->receptionist->eraseClient( this );
			return ( 0 );
		}
		if ( resSendStatus == Response::SENT )
		{
			Log::Debug( std::string("onEventWriteSocket was Response::SENT") );
			nextRequest();
		}
	}
	return ( 0 );
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
	if ( tevent.filter == EVFILT_READ )
		return ( onEventRead( tevent ) );
	if ( tevent.filter == EVFILT_WRITE )
		return ( onEventWrite( tevent ) );
	if ( tevent.filter == EVFILT_PROC )
		return ( onEventProcExit( tevent ) );
	if ( tevent.filter == EVFILT_TIMER )
		return ( onEventProcTimeout( tevent ) );
	return ( 0 );
}
