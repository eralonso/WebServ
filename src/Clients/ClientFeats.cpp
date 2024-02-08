/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientFeats.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:20:14 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:23:36 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Router.hpp"

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