/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientFeats.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:20:14 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:57:16 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Router.hpp"
#include "Receptionist.hpp"

Response	*Client::createResponse( void )
{
	if ( !this->res )
		this->res = new Response;
	return ( this->res );
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
			Log::Debug("Client::manageRecv: remainning received length [" + SUtils::longToString(this->received.length()) +  "]");
			cont = req->processRecv();
			if ( req->isCompleteRecv() )
				cont = false;
			if ( req->isBadRequest() )
			{
				Log::Debug( "Client [ " + SUtils::longToString( this->socket ) + " ] -> bad Request" );
				setEventWriteSocket();
				enableEventReadSocket( false );
			}
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


int	Client::readRequest( socket_t clientFd, std::string& readed )
{
	char	buffer[ BUFFER_SIZE + 1 ];
	ssize_t	amount;

	amount = recv( clientFd, buffer, BUFFER_SIZE, MSG_DONTWAIT );
	Log::Debug("on recv on [" + SUtils::longToString(clientFd) + "]");
	Log::Debug( "amount: " + SUtils::longToString( amount ) );
	if (amount > 0)
	{
		std::string content(buffer, amount);
		Log::Debug("content read on [" + SUtils::longToString(clientFd) 
			+ "]: \n----------------------------\n"
			+ content
			+ "-------------------------");
		readed += content;
	}
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
	this->cgiHeaderReached = -1;
	this->cgiContentLength = -1;
	this->cgiOutput = std::string("");
}

void	Client::nextRequest( void )
{
	bool	badRequest = this->front()->isBadRequest();

	Log::Debug( std::string("nextRequest"));
	this->front()->logStatus();
	if ( ! this->front()->isReceiving() || badRequest )
	{
		this->eraseRequest();
		this->reset();
	}
	else
	{
		delete this->res;
		this->res = NULL;
		this->responseSent = true;
	}
	enableEventWriteSocket(false);
	Log::Debug( std::string("nextRequest deleting Request & Response"));
	
	if ( ( this->size() > 0 || this->getPendingSize() > 0 || this->getKeepAlive() ) && !badRequest )
		this->enableEventReadSocket( true );
	else
		this->receptionist->eraseClient( this );
}

void	Client::reset( void )
{
	if ( this->res )
		delete this->res;
	if ( this->fileFd > 0 )
		close( this->fileFd );
	this->fileFd = -1;
	this->res = NULL;
	this->requestBodyRemain = 0;
	this->responseBodyRemain = 0;
	this->responseHeaderSent = false;
	this->responseSent = false;
	this->readEOF = false;
	this->writeEOF = false;
	if ( this->pipeCgiWrite > 0 )
		close( this->pipeCgiWrite );
	if ( this->pipeCgiRead > 0 )
		close( this->pipeCgiRead );
	this->resetCgiOperation();	
}

ssize_t	Client::CgiFindHeaderReached()
{
	size_t pos;

	pos = 0;
	do
	{
		pos = this->cgiOutput.find("\n", pos);
		if ( pos != std::string::npos )
		{

			if ( cgiOutput[pos + 1] == '\n' )
				return (pos + 1);
			if ( cgiOutput[pos + 1] && cgiOutput[pos + 1] <= ' ' && 
						cgiOutput[pos + 2] == '\n' )
				return (pos + 2);
		} 
		pos += pos != std::string::npos ? 1 : 0;
	} while (pos != std::string::npos );
	return (-1);
}

ssize_t	Client::CgiFindContentLength( )
{
	size_t pos;
	std::string str;

	str = SUtils::toLower(cgiOutput.substr(0, cgiHeaderReached));
	pos = str.find("content-length:");
	if (pos != std::string::npos)
		return (SUtils::atol( str.substr(pos + 15) ));
	return (-1);
}

bool	Client::isCgiContentLengthOverpass( void ) const
{
	return ((this->cgiHeaderReached != -1 && this->cgiContentLength != -1 
			&& (this->cgiOutput.length() - (size_t)this->cgiHeaderReached) 
				>= (size_t)this->cgiContentLength));
}

void	Client::adjustCgiContentLengthOverpass( void )
{
	size_t	passed_pos;
	size_t	passed_size;

	if (isCgiContentLengthOverpass())
	{
		passed_pos = this->cgiHeaderReached + this->cgiContentLength;
		passed_size = this->cgiOutput.length() - passed_pos;
		this->cgiOutput.erase(passed_pos, passed_size);
	}
}
