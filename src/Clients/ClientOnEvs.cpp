/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientOnEvs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:12:13 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:13:41 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Receptionist.hpp"

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
