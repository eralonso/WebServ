/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestStMach.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:44:22 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:18:58 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <Client.hpp>
#include <Router.hpp>

bool	Request::processLineOnFdBond( const std::string &line )
{
	size_t len = line.length();

	if ( len == 0 || ( len == 1 && line[ 0 ] <= ' ' ) )
	{
		this->status = RECVD_START;
		return ( true );
	}
	parseFirstLine( line );
	if ( this->badRequest == false )
		this->status = RECVD_REQ_LINE;
	return ( true );
}

bool	Request::processLineOnRecvdStart( const std::string &line )
{
	parseFirstLine( line );
	if ( this->badRequest == false )
		this->status = RECVD_REQ_LINE;
	return ( true );
}

bool	Request::processLineOnRecvdReqLine( const std::string &line )
{
	size_t		len = line.length();

	if ( len == 0 || ( len == 1 && line[ 0 ] <= ' ' ) )
	{
		this->status = RECVD_HEADER;
		checkKeepAlive();
		if (!updateServerConfig())
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		updateLocation();
		this->useCgi = checkCgiInRoute();
		maxBodySize = svr->getMaxBodySize( this->lc );
		if (svr->getIsAllowedMethod( this->lc, this->method ) == false)
			return ( setError( HTTP_NOT_ALLOWED_CODE ) );
		updateFilePaths();
		if ( checkChunked() )
		{
			Router::processRequestHeaderReceived( *this );
			return ( true );
		}
		processOnReceivingBody();
		Router::processRequestHeaderReceived( *this );
		return ( true );
	}
	parseHeader( line );
	return ( true );
}

bool	Request::processOnReceivingBody( void )
{
	size_t		contentSize;
	ssize_t		take;
	std::string	data;
	Header		*clHead = this->headers.firstWithKey( "Content-Length" );

	if ( clHead != NULL )
	{
		contentSize = SUtils::atol( clHead->getValue().c_str() );
		if ( maxBodySize != 0 && contentSize > maxBodySize )
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		take = contentSize - this->recvBodyLength;
		this->client->getNChars( data, take );
		this->body += data;
		this->recvBodyLength += data.length();
		if ( this->recvBodyLength >= contentSize )
			this->status = RECVD_ALL;
	}
	else
	{
		this->status = RECVD_ALL;
	}
	return ( !this->isCompleteRecv() );
}

bool	Request::processLineOnRecvdHeader( const std::string &line )
{
	Header*	clHead = this->headers.firstWithKey( "Content-Length" );
	size_t	contentSize;

	this->body += line + '\n';
	if ( clHead != NULL )
	{
		contentSize = SUtils::atol( clHead->getValue().c_str() );
		if (maxBodySize != 0 && contentSize > maxBodySize)
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		if ( this->recvBodyLength >= contentSize )
			this->status = RECVD_ALL;
		return ( this->client->getPendingSize() > 0 );
	}
	this->status = RECVD_ALL;
	return ( true );
}

bool	Request::processLineOnRecvdChunkSize( const std::string &line )
{
	( void )line;
	return ( true );
}

bool	Request::processLineOnRecvdChunk( const std::string &line )
{
	size_t len = line.length();

	if ( ( ( len == 1 || ( len == 2 && line[ 1 ] <= ' ' ) ) && line[ 0 ] == '0' ) )
	{
		this->chunkSize = 0;
		this->status = RECVD_LAST_CHUNK;
		return ( true );
	}
	this->chunkSize = SUtils::atolhex( line.c_str() );
	if ( chunkSize > 0 )
	{
		this->status = RECVD_CHUNK_SIZE;
		return ( true );
	}
	this->status = RECVD_LAST_CHUNK;
	return ( true );
}

bool	Request::processOnReceivingChunk( void )
{
	size_t		len = this->chunkSize;
	std::string	chunk;
	std::string	line;

	if ( this->client->getPendingSize() < this->chunkSize + 2 )
		return ( false );
	this->client->getNChars( chunk, len );
	this->client->getLine( line );
	this->body += chunk;
	this->status = RECVD_CHUNK;
	return ( true );
}

bool	Request::processLineOnRecvdLastChunk( const std::string &line )
{
	size_t len = line.length();

	if ( len == 0 || ( len == 1 && line[ 0 ] <= ' ' ) )
	{
		this->status = RECVD_ALL;
		return ( true );
	}
	Log::Error( "[" + line + "]" );
	return ( setError( HTTP_BAD_REQUEST_CODE ) );
}

bool	Request::processLine( const std::string &line )
{
	switch ( status )
	{
		case IDLE:
			return ( false );
		case FD_BOND:
			return ( processLineOnFdBond( line ) );
		case RECVD_START:
			return ( processLineOnRecvdStart( line ) );	
		case RECVD_REQ_LINE:
			return ( processLineOnRecvdReqLine( line ) );
		case RECVD_HEADER:
			return ( processLineOnRecvdHeader( line ) );
		case RECVD_CHUNK_SIZE:
			return ( processLineOnRecvdChunkSize( line ) );
		case RECVD_CHUNK:
			return ( processLineOnRecvdChunk( line ) );
		case RECVD_LAST_CHUNK:
			return ( processLineOnRecvdLastChunk( line ) );
		case RECVD_ALL:
			Log::Debug( "processLine with RECVD_ALL" );
			return ( false );
		case CGI_LAUNCHED:
			Log::Debug( "processLine with CGI_LAUNCHED" );
			return ( false );
		case RESP_RENDERED:
			Log::Debug( "processLine with RESP_RENDERED" );
			return ( false );
	}
	return ( false );
}

bool	Request::processRecv( void )
{
	bool		cont = true;
	std::string	line;

	if ( this->status == IDLE || this->client == NULL )
		return ( false );
	this->logStatus();
	while ( cont && !this->isCompleteRecv() && this->client->getPendingSize() > 0 )
	{
		if ( this->status == RECVD_HEADER )
			cont = processOnReceivingBody();
		else if ( this->status == RECVD_CHUNK_SIZE )
			return ( processOnReceivingChunk() );
		else if ( this->client->getLine( line ) )
			cont = processLine( line );
		else
			return ( false );
	}
	return ( !this->isCompleteRecv() );
}
