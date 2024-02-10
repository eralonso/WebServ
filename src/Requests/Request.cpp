/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/10 11:28:31 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <Client.hpp>

size_t	Request::id_counter = 0;

Request::Request( void )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->badRequest = false;
	this->client = NULL;
	this->status = IDLE;
	this->useCgi = false;
	this->error = 0;
	this->svr = NULL;
	this->lc = NULL;
	this->maxBodySize = 1 << 20;
	this->redir = false;
	this->outputLength = 0;
	this->recvBodyLength = 0;
}

Request::Request( Client *cli )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->badRequest = false;
	this->client = cli;
	this->status = FD_BOND;
	this->useCgi = false;
	this->error = 0;
	this->svr = NULL;
	this->lc = NULL;
	this->maxBodySize = 1 << 20;
	this->redir = false;
	this->outputLength = 0;
	this->recvBodyLength = 0;
	// Log::Debug("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::~Request( void ) 
{
	// CgiExecutor::purgeDiscardedRequest(this);
}

Request::Request( const Request& b )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->badRequest = false;
	this->chunkSize = b.chunkSize;
	this->client = b.client;
	this->status = b.status;
	this->method = b.method;
	this->url = b.url;
	this->route = b.route;
	this->query = b.query;
	this->headers = b.headers;
	this->body = b.body;
	this->cgiOutput = b.cgiOutput;
	this->useCgi = b.useCgi;
	this->error = b.error;
	this->routeChain = b.routeChain;
	this->document = b.document;
	this->docExt = b.docExt;
	this->svr = b.svr;
	this->lc = b.lc;
	this->maxBodySize = b.maxBodySize;
	this->redir = b.redir;
	this->uriRedir = b.uriRedir;
	this->outputLength = b.outputLength;
	this->recvBodyLength = b.recvBodyLength;
}

Request&	Request::operator=( const Request& b )
{
	if ( this != &b )
	{
		this->badRequest = false;
		this->chunkSize = b.chunkSize;
		this->client = b.client;
		this->status = b.status;
		this->method = b.method;
		this->url = b.url;
		this->route = b.route;
		this->query = b.query;
		this->headers = b.headers;
		this->body = b.body;
		this->cgiOutput = b.cgiOutput;
		this->useCgi = b.useCgi;
		this->error = b.error;
		this->routeChain = b.routeChain;
		this->document = b.document;
		this->docExt = b.docExt;
		this->svr = b.svr;
		this->lc = b.lc;
		this->maxBodySize = b.maxBodySize;
		this->redir = b.redir;
		this->uriRedir = b.uriRedir;
		this->outputLength = b.outputLength;
		this->recvBodyLength = b.recvBodyLength;
	}
	return ( *this );
}

int	Request::bindClient( Client* cli )
{
	this->client = cli;
	this->status = FD_BOND;
	return ( this->status );
}

bool	Request::checkKeepAlive( void )
{
	Header	*con = headers.firstWithKey( "Connection" );

	if ( !this->client )
		return ( false );
	if ( con != NULL )
		this->client->setKeepAlive( con->getValue() == "keep-alive" );
	else
		this->client->setKeepAlive( false );
	return ( con != NULL );
}

bool	Request::checkEmptyContent( size_t& size )
{
	Header	*clHead = headers.firstWithKey( "Content-Length" );

	if ( !clHead )
		this->status = RECVD_ALL;
	else
		size = SUtils::atol( clHead->getValue().c_str() );
	return ( this->status == RECVD_ALL );
}

std::string	Request::toString( void )
{
	std::string ret = getMethod() + " " + this->route + " " + this->protocol + "\n";

	ret += this->headers.toString();
	ret += HEADER_SEP;
	ret += this->body;
	return ( ret );
}

void	Request::logStatus( void )
{
	switch ( status )
	{
		case IDLE:
			Log::Debug( "status = IDLE" );
			break ;
		case FD_BOND:
			Log::Debug( "status = FD_BOND" );
			break ;
		case RECVD_START:
			Log::Debug( "status = RECVD_START" );
			break ;
		case RECVD_REQ_LINE:
			Log::Debug( "status = RECVD_REQ_LINE" );
			break ;
		case RECVD_HEADER:
			Log::Debug( "status = RECVD_HEADER" );
			break ;
		case RECVD_CHUNK_SIZE:
			Log::Debug( "status = RECVD_CHUNK_SIZE" );
			break ;
		case RECVD_CHUNK:
			Log::Debug( "status = RECVD_CHUNK" );
			break ;
		case RECVD_LAST_CHUNK:
			Log::Debug( "status = RECVD_LAST_CHUNK" );
			break ;
		case RECVD_ALL:
			Log::Debug( "status = RECVD_ALL" );
			break ;
		case CGI_LAUNCHED:
			Log::Debug( "status = CGI_LAUNCHED" );
			break ;
		case RESP_RENDERED:
			Log::Debug( "status = RESP_RENDERED" );
			break ;
		default:
			Log::Debug( "status = " + SUtils::longToString( status ) );
	}
}
