/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientOnEvs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:12:13 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/09 15:10:46 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Receptionist.hpp"

int	Client::onEventProcExit( Event& tevent )
{
	Log::Info( "onEventProcExit" );
	this->deleteEventProcTimeout(tevent.ident);
	this->cgiTimeout = false;
	this->cgiFinished = true;
	this->performCgiCompletion();
	return (0);
}

int	Client::onEventProcTimeout( Event& tevent )
{
	Log::Info( "onEventProcTimeout" );
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

	Log::Info( "onEventReadSocket" );
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

	Log::Info( "onEventReadFile" );
	Log::Info( "ident: " + SUtils::longToString( tevent.ident ) );
	Log::Info( "filter: " + SUtils::longToString( tevent.filter ) );
	createResponse();
	if (amountToRead > (size_t)tevent.data)
		amountToRead = (size_t)tevent.data;
	actualRead = read(tevent.ident, buffer, amountToRead);
	std::string content(buffer, actualRead);
	if (this->res)
		this->res->setBody( this->res->getBody() + content );
	if(amountToRead - actualRead == 0)
	{
		this->res->updateResString();
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
		this->fileFd = -1;
	}
	return ( 0 );
}

int	Client::onEventReadPipe( Event& tevent )
{
	char	buffer[ BUFFER_SIZE ];
	size_t	amountToRead = BUFFER_SIZE;
	size_t	actualRead;

	Log::Info( "onEventReadPipe" );
	createResponse();
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
			this->res->updateResString();
		}
		this->setEventWriteSocket();
		this->readEOF = true;
		close(tevent.ident);
		this->pipeCgiRead = -1;
		this->performCgiCompletion();
	}
	return ( 0 );
}

int	Client::onEventWriteSocket( Event& tevent )
{
	int			resSendStatus;

	(void)tevent;
	Log::Info( "onEventWriteSocket" );
	createResponse();
	if ( this->res )
	{
		if ( this->res->getSendStatus() == Response::GETTING_DATA )
			Router::updateResponse( *this->res, *this->front(), *this );
		resSendStatus = this->res->sendResponse( this->socket );
		if ( resSendStatus == Response::ERROR || resSendStatus == Response::SENT )
		{
			Log::Info( std::string("onEventWriteSocket was ") + (resSendStatus == Response::ERROR ? "Response::ERROR" : "Response::SENT") );
			nextRequest();
		}
	}
	return ( 0 );
}

int	Client::onEventWriteFile( Event& tevent )
{
	size_t	size;

	Log::Info( "onEventWriteFile" );
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
	if (req->isCompleteRecv() && req->getBodyLength() == 0)
	{
		this->writeEOF = true;
		Router::updateResponse( *this->res, *this->front(), *this );
		this->res->updateResString();
		setEventWriteSocket();
		close(tevent.ident);
		this->fileFd = -1;
	}
	return 0;
}

int	Client::onEventWritePipe( Event& tevent )
{
	Log::Info( "onEventWritePipe" );
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
		this->writeEOF = true;
		this->performCgiCompletion();
		close(tevent.ident);
		this->pipeCgiWrite = -1;
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
