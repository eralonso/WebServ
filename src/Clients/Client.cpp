/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 12:46:14 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Receptionist.hpp"
#include "Client.hpp"
#include "Router.hpp"

size_t	Client::id_counter = 0;

Client::Client( void )
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->keepAlive = false;
	this->pending = 0;
	this->socket = -1;
	this->polls = NULL;
	// Log::Info("Created client id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Client::Client( socket_t pollsocket, WSPoll& polls )
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->keepAlive = false;
	this->pending = 0;
	this->socket = pollsocket;
	this->polls = &polls;
	// Log::Info("Created client id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Client::~Client( void ) {}

Client::Client( const Client& b ): Requests()
{
	this->id = Client::id_counter;
	Client::id_counter++;
	this->socket = b.socket;
	this->pending = b.pending;
	this->received = b.received;
}

Client&	Client::operator=( const Client& b )
{
	if ( this != &b )
	{
		this->socket = b.socket;
		this->pending = b.pending;
		this->received = b.received;
	}
	return ( *this );
}

int Client::bindClientPoll( socket_t pollsocket )
{
	this->socket = pollsocket;
	return ( 0 );
}

socket_t	Client::getClientSocket( void ) const
{
	return ( this->socket );
}

size_t	Client::getId( void ) const
{
	return ( this->id );
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
	while ( cont && !fail && getLine( line ) )
	{
		req = findRecvRequest();
		if ( req != NULL )
			cont = req->processLine( line );
		else
		{
			req = Requests::appendRequest( this );
			if ( req != NULL )
				cont = req->processLine( line );
			else
				fail = true;
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
	Response	*res = NULL;
	int			count = 0;

	while ( ( req = findReadyToSendRequest() ) )
	{
		res = Router::getResponse( req );
		if ( res && sendResponse( res->toString() ) )
		{
			count++;
			Requests::eraseRequest();
			delete res;
			res = NULL;
		}
	}
	return ( count );
}

bool	Client::getKeepAlive( void ) const
{
	return ( this->keepAlive );
}

int	Client::sendResponse( std::string resp )
{
	if ( this->socket >= 0 )
		return ( Receptionist::sendResponse( this->socket, resp ) );
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

size_t	Client::getPendingSize( void ) const
{
	return ( this->received.size() - this->pending );
}

// int Client::setDummyRecv()
// {
// 	std::string init("GET / Http/1.1\r\n");
// 	init += std::string("Host: localhost\r\n");
// 	init += std::string("Content-Type: text/xml; charset=utf-8\r\n");
// 	init += std::string("Content-Lenght: 6\r\n");
// 	init += std::string("Accept-Language: en-us\r\n");
// 	init += std::string("Accept-Encoding: gzip, deflate\r\n");
// 	init += std::string("\r\n");
// 	init += std::string("NoBody\r\n");
// 	received += init;
// 	return (1);
// }

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

void	Client::allowPollWrite( bool value )
{
	struct pollfd	*clientPoll = NULL;

	if ( this->polls != NULL )
	{
		// polls->allowPollWrite(socket, value);
		try
		{
			clientPoll = &( polls->operator[]( socket ) );
		}
		catch ( std::out_of_range& e )
		{ 
			Log::Info( "ClientPoll for [ " \
				+ SUtils::longToString( this->socket ) \
				+ " ]: not found");
			return ;
		}
		if ( value )
			clientPoll->events |= POLLOUT;
		else 
			clientPoll->events &= ~POLLOUT;
	// if (polls)
	// 	polls->allowPollWrite(socket, value);
	}
	else
		Log::Error( "Polls not found on Client " \
			+ SUtils::longToString( this->id ) );
}

bool	Client::checkPendingToSend( void )
{
	LogId();
	if ( Requests::checkPendingToSend() )
	{
		allowPollWrite( true );
		return ( true );
	}
	return ( false );
}
