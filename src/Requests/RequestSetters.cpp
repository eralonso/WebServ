/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestSetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:36:29 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/09 13:18:05 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <Router.hpp>

void	Request::setBody( const std::string& content )
{
	this->body = content;
}

void	Request::appendBody( const std::string& content )
{
	this->body += content;
}

void	Request::eraseBody( size_t size )
{
	if (size > body.size())
		size = body.size();
	body.erase(0, size);
}

void	Request::setReadyToSend( void )
{
	if ( this->status == RECVD_ALL || this->status == CGI_LAUNCHED )
		this->status = RESP_RENDERED;
}

void Request::setReceivedAll()
{
	status = RECVD_ALL;
}

void	Request::setCgiLaunched( void )
{
	if ( this->status == RECVD_ALL )
		this->status = CGI_LAUNCHED;
}

void	Request::setCgiOutput( std::string str )
{
	this->cgiOutput = str;
}

void	Request::setUseCgi( bool value )
{
	this->useCgi = value;
}

void	Request::setOutput( std::string str )
{
	this->output = str;
}

bool	Request::setError( int value )
{
	Client*		cli;
	Response*	res;

	if ( value >= HTTP_BAD_REQUEST_CODE )
	{
		this->badRequest = true;
		this->status = RECVD_ALL;
		cli = this->getClient();
		res = cli->createResponse();
		res->setStatus(value);
		cli->setEventWriteSocket();
		cli->enableEventReadSocket(false);
	}
	this->error = value;
	return ( this->badRequest != true );
}

void	Request::setStatus( int value )
{
	this->error = value;
}

void Request::setDefaultFavicon(void)
{
	this->route = "/favicon.svg";
	parseRoute();
	setError(0);
	updateFilePaths();
}

void	Request::setRedir( bool isRedir )
{
	this->redir = isRedir;
}

void	Request::setUriRedir( std::string uriRedirection )
{
	this->uriRedir = uriRedirection;
}

void	Request::setRedirection( std::string uri, int code )
{
	this->redir = true;
	this->uriRedir = uri;
	setStatus( code );
}

void	Request::setOutputLength( size_t size )
{
	this->outputLength = size;
}

void	Request::setDocExt( std::string ext )
{
	this->docExt = ext;
}

void	Request::setCompletedRequest( void )
{
	this->setError( HTTP_OK_CODE );
	this->setReadyToSend();
}

void	Request::setTimeoutedRequest( void )
{
	this->setError( HTTP_GATEWAY_TIMEOUT_CODE );
	Router::checkErrorRedir( this->getError(), *this );
	Router::checkErrorBody( *this, this->getError() );
	this->setReadyToSend();
}