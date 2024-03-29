/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:02 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:33:46 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Response.hpp"
#include "Events.hpp"

Response::Response( void ): server( SERVER ), isCgi( false ), sendPos( 0 ), sendState( GETTING_DATA )
{
}

Response::~Response( void ) {}

Response::Response( const Response& b )
{
	this->method = b.method;
	this->route = b.route;
	this->query = b.query;
	this->headers = b.headers;
	this->body = b.body;
	this->isCgi = b.isCgi;
	this->server = b.server;
	this->sendPos = b.sendPos;
	this->sendState = b.sendState;
}

Response&	Response::operator=( const Response& b )
{
	if ( this != &b )
	{
		this->method = b.method;
		this->route = b.route;
		this->query = b.query;
		this->headers = b.headers;
		this->body = b.body;	
		this->isCgi = b.isCgi;
		this->server = b.server;
		this->sendPos = b.sendPos;
		this->sendState = b.sendState;
	}
	return ( *this );
}

void	Response::setServer( std::string server )
{
	this->server = server;
	headers.replace( "Server", server );
}

void	Response::setProtocol( std::string protocol )
{
	this->protocol = protocol;
}

void	Response::setMethod( std::string method )
{
	this->method = method;
}

void	Response::setRoute( std::string route )
{
	this->route = route;
}

void	Response::setStatus( unsigned int status )
{
	this->status = status;
}

void	Response::setQuery( std::string query )
{
	this->query = query;
}

void	Response::setHeaders( Headers headers )
{
	this->headers = headers;
}

void	Response::setBody( std::string content )
{
	Header	h( "Content-Length", SUtils::longToString( content.length() ) );

	this->body = content;
	this->headers.replace( h );
}

void	Response::setBodyLength( size_t len )
{
	Header	h( "Content-Length", SUtils::longToString( len ) );

	this->headers.replace( h );
}

void Response::setIsCgi(bool value)
{
	isCgi = value;
}

size_t Response::increaseSendPos(size_t value)
{
	this->sendPos += value;
	return ( this->sendPos );
}

void Response::appendHeader(Header header)
{
	this->headers.append( header );
}

void Response::replaceHeader(Header header)
{
	this->headers.replace( header );
}

std::string	Response::getServer( void ) const
{
	return ( this->server );
}

std::string	Response::getProtocol( void ) const
{
	return ( this->protocol );
}

std::string	Response::getMethod( void ) const
{
	return ( this->method );
}

std::string	Response::getRoute( void ) const
{
	return ( this->route );
}

unsigned int	Response::getStatus( void ) const
{
	return ( this->status );
}

std::string	Response::getResult( void ) const
{
	/* Informational responses (100 – 199)
	Successful responses (200 – 299)
	Redirection messages (300 – 399)
	Client error responses (400 – 499)
	Server error responses (500 – 599)*/
	//int		hundreds = this->status / 100;
	//bool	informational = ( hundreds == 1 );
	//bool	successful = ( hundreds == 2 );
	//bool	redirection = ( hundreds == 3 );
	//bool	OK = ( informational || successful || redirection );
	//(void)OK;
	return ( StatusCodes::decode( this->status ) );
}

std::string	Response::getQuery( void ) const
{
	return ( this->query );
}

const Headers&	Response::getHeaders( void ) const
{
	return ( this->headers );
}

Header	*Response::getHeader( std::string key )
{
	return ( this->headers.firstWithKey( key ) );
}

size_t	Response::getContentLength( void ) const
{
	return ( this->body.length() );
}

std::string	Response::getBody( void ) const
{
	return ( this->body );
}

bool	Response::getIsCgi() const
{
	return (isCgi);
}

size_t	Response::getSendPos( void ) const
{
	return ( this->sendPos );
}

const std::string&	Response::getResString( void ) const
{
	return ( this->resString );
}

Response::t_sendStatus	Response::getSendStatus( void ) const
{
	return ( this->sendState );
}

void	Response::updateResString( void )
{
	Log::Debug("updateResString");
	if (!isCgi)
	{
		Log::Debug("updateResString is set as: ");
		this->resString = this->protocol + " " + SUtils::longToString( this->status );
		this->resString += " " + getResult() + HEADER_SEP;
		this->resString += this->headers.toString();
		this->resString += HEADER_SEP;
		this->resString += this->body;
		Log::Debug(this->resString);
		return ;
	}
	this->resString = this->body;
}

int	Response::sendResponse( Event& tevent )
{
	size_t		threshold;
	const char	*str;
	size_t		size;
	ssize_t		ret;
	size_t		pos;

	if ( this->sendState == Response::GETTING_DATA )
		this->sendState = Response::SENDING;
	pos = this->sendPos;
	size = this->resString.size() - pos; 
	str = this->resString.c_str();
	threshold = size > SEND_BUFFER_SIZE ? SEND_BUFFER_SIZE : size;
	if (this->sendState == Response::SENT || threshold == 0)
		return ( this->sendState );
	if ( ( ret = send( tevent.ident, str + pos, threshold, MSG_DONTWAIT ) ) < 0 )
	{
		Log::Info( "Client [ " + SUtils::longToString( tevent.ident ) \
			+ " ] aborted connection" );
		return ( Response::ERROR );
	}
	pos = increaseSendPos( ret );
	if ( pos >= this->resString.size() )
	{
		this->sendState = Response::SENT;
		Log::Debug( "Response sent [ " + SUtils::longToString( tevent.ident ) + " ]" );
	}
	return ( this->sendState );
}