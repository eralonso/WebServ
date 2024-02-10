/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/10 11:22:31 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Response.hpp"
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
	Log::Error( "Client destroy [ " + SUtils::longToString( this->socket ) + " ]" );
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

void	Client::LogId( void ) const
{
	Log::Info( "Client id: " + SUtils::longToString( id ) );
}
