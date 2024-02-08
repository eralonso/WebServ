/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestGetters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:29:33 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:40:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Utils.hpp>
#include <SplitString.hpp>
#include <Request.hpp>

bool	Request::getExtensionForPath( std::string path, std::string& ext )
{
	size_t	pos;

	pos = path.find_last_of( "." );
	if ( pos == std::string::npos )
		return ( false );
	ext = path.substr( pos + 1 );	
	return ( true );
}

Request::t_status	Request::getStatus( void ) const
{
	return ( this->status );
}

int Request::getError( void ) const
{
	return ( this->error );
}

std::string	Request::getCgiOutput( void ) const
{
	return ( this->cgiOutput );
}

bool	Request::getUseCgi( void ) const
{
	return ( useCgi );
}

Client	*Request::getClient( void ) const
{
	return ( this->client );
}

std::string	Request::getProtocol( void ) const
{
	return ( this->protocol );
}

std::string	Request::getMethod( void ) const
{
	return ( this->method );
}

std::string	Request::getRoute( void ) const
{
	return ( this->route );
}

StringVector	Request::getRouteChaine( void ) const
{
	return ( this->routeChain );
}

std::string	Request::getRouteChaineString( void ) const
{
	size_t		it = 0;
	size_t		size = this->routeChain.size();
	std::string	chain( "/" );

	while ( it < size )
	{
		chain += this->routeChain[ it ] + "/";
		it++;
	}
	return ( chain );
}

std::string	Request::getDocument( void ) const
{
	return ( this->document );
}

bool	Request::isDocumentNPH( void ) const
{
	bool nph = ( this->document.size() > 4
		&& (this->document[0] == 'n' || this->document[0] == 'N')
		&& (this->document[1] == 'p' || this->document[1] == 'P')
		&& (this->document[2] == 'h' || this->document[2] == 'H')
		&& (this->document[3] == '-') );
	return ( nph );
}

std::string	Request::getDocExt( void ) const
{
	return ( this->docExt );
}

std::string	Request::getQuery( void ) const
{
	return ( this->query );
}

const Headers&	Request::getHeaders( void ) const
{
	return ( this->headers );
}

std::string	Request::getHost( void ) const
{
	if (this->routeHost.size() > 0)
		return (this->routeHost);
	const Header* h = this->headers.firstWithKey( "Host" );
	if (h)
	{
		std::string value = h->getValue();
		std::vector<std::string> tokens = SplitString::split(value,
										std::string(":"));
		if (tokens.size() > 0)		
			return (tokens[0]);
	}
	return std::string("127.0.0.1");
}

std::string Request::getPort( void ) const
{
	if (this->routePort.size() > 0)
		return (this->routePort);
	const Header* h = headers.firstWithKey("Host");
	if (h)
	{
		std::string value = h->getValue();
		std::vector<std::string> tokens = SplitString::split(value,
										std::string(":"));
		if (tokens.size() > 1)		
			return (tokens[1]);
	}
	return std::string("80");
}

bool Request::getHostPort(std::string& host, std::string& port) const
{
	std::string defHost("127.0.0.1");
	std::string defPort("8000");
	if (this->routeHost.size() > 0 || this->routePort.size() > 0)
	{
		Log::Info("Request::getHostPort");
		if (this->routeHost.size() == 0)
			this->routeHost.copy((char*)defHost.c_str(), defHost.size());
		if (this->routePort.size() == 0)
			this->routePort.copy((char*)defPort.c_str(), defPort.size());
		host = this->routeHost;
		port = this->routePort;		
		return true;
	}
	const Header* h = headers.firstWithKey("Host");
	if (h)
	{
		std::string value = h->getValue();
		std::vector<std::string> tokens = SplitString::split(value,
										std::string(":"));
		if (tokens.size() > 0)
			host = tokens[0];
		else
			host = defHost;
		if (tokens.size() > 1)
			port = tokens[1];
		else
			port = defPort;
		return true;
	}
	else
		return false;
}

std::string Request::getHeaderWithKey(const std::string& key)
{
	Header* h = headers.firstWithKey(key);

	if ( h != NULL )
		return ( h->getValue() );
	else
		return ( "Unknown" );
}

size_t	Request::getBodyLength( void ) const
{
	return ( this->body.length() );
}

const std::string&	Request::getBody( void ) const
{
	return ( this->body );
}

std::string	Request::getBodyHead( size_t size ) const
{
	if (body.size() < size)
		size = body.size();
	return (this->body.substr(0, size));
}

size_t	Request::getId( void ) const
{
	return ( this->id );
}

std::string	Request::getFilePathWrite( void ) const
{
	return ( this->filePathWrite );
}

std::string	Request::getFilePathRead( void ) const
{
	return ( this->filePathRead );
}

const Server	*Request::getServer( void ) const
{
	return ( this->svr );
}

const Location	*Request::getLocation( void ) const
{
	return ( this->lc );
}

std::string	Request::getFinalPath( void ) const
{
	std::string	routeWithoutFile = getRouteChaineString();

	if ( this->svr != NULL )
		return ( this->svr->getFinalPath( routeWithoutFile, this->lc ) );
	return ( ConfigUtils::pathJoin( ".", routeWithoutFile ) );
}

std::string	Request::getCgiBinary( std::string ext ) const
{
	if ( this->svr != NULL )
		return ( this->svr->getCgiBinary( ext, this->lc ) );
	return ( "" );
}

std::string	Request::getOutput( void ) const
{
	return ( this->output );
}

bool	Request::getErrorPage( int error, std::string& uriRedir )
{
	if ( this->svr != NULL )
		return ( this->svr->getErrorPageWithCode( error, uriRedir, this->lc ) );
	return ( false );
}
bool	Request::getRedir( void ) const
{
	return ( this->redir );
}

std::string	Request::getUriRedir( void ) const
{
	return ( this->uriRedir );
}

size_t	Request::getOutputLength( void ) const
{
	return ( this->outputLength );
}

std::string	Request::getPathInfo( void ) const
{
	return ( this->pathInfo );
}

bool	Request::isReadyToSend( void ) const
{
	return ( this->status == RESP_RENDERED );
}

bool	Request::isCompleteRecv( void ) const
{
	return ( this->status == RECVD_ALL );
}

bool	Request::isCgiLaunched( void ) const
{
	return ( this->status == CGI_LAUNCHED );
}

bool	Request::isReceiving( void ) const
{
	return ( this->status < RECVD_ALL );
}

bool	Request::isDirectiveSet( std::string directive ) const
{
	if ( this->lc != NULL )
		return ( this->lc->isSet( directive ) );
	return ( false );	
}

bool	Request::isAutoindexAllow( void ) const
{
	if ( this->svr != NULL )
		return ( this->svr->isAutoindexAllow( this->lc ) );
	return ( false );
}

std::string Request::getCookies( void ) const
{
	return (headers.getCookies());	
}
