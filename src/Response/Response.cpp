/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:02 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/17 11:52:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Response.hpp"

Response::Response( void ): server( "OREGINX" ), isCgi( false ) {}

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

void Response::appendHeader(Header header)
{
	this->headers.append( header );
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

std::string	Response::toString( void ) const
{
	std::string ret;

	if (!isCgi)
	{
		// Log::Info("Response::toString compose response");
		ret = this->protocol + " " + SUtils::longToString( this->status );
		ret += " " + getResult() + HEADER_SEP;
		ret += this->headers.toString();
		ret += HEADER_SEP;
		ret += this->body;
		// Log::Info(ret);
		return ( ret );
	}
	// Log::Info("Response::toString use only body content");
	// Log::Info(body);
	return (body);
}
