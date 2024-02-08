/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:47:43 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:51:23 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Utils.hpp>
#include <SplitString.hpp>
#include <Request.hpp>

void	Request::parseQueryStringFromRoute( void )
{
	StringVector			tokens;
	size_t					len;

	tokens = SplitString::split( route, "?" );
	len = tokens.size();
	//if ( len > 2 )
	//{
	//	Log::Error( "Request query string invalid" );
	//	this->error = HTTP_BAD_REQUEST_CODE;
	//	this->badRequest = true;
	//	return ;
	//}
	this->url = SUtils::trim( tokens[ 0 ] );
	this->route = this->url;
	if ( len > 1 )
		this->query = SUtils::trim( tokens[ 1 ] );
}

bool	Request::parseDropHttp(StringVector& tokens,  size_t& tokensSize, bool& httpDropped)
{
	tokens.erase(tokens.begin(), tokens.begin() + 1);
	httpDropped = true;
	if (httpDropped && (tokens[0].size() < 2 || tokens[0][0] != '/' || tokens[0][1] != '/') )
	{
		Log::Error( "Request route invalid, after http: must follow //" );
		this->setError(HTTP_BAD_REQUEST_CODE);
		return true;
	}
	tokens[0].erase(tokens[0].begin(), tokens[0].begin() + 2);
	tokensSize--;
	if (tokensSize > 1)
		this->route = tokens[0] + ":" + tokens[1];
	else
		this->route = tokens[0];
	return false;	
}

void	Request::parseHostPortFromRoute( void )
{
	StringVector	tokens;
	size_t			tokensSize;
	bool			httpDropped = false;

	tokens = SplitString::split( this->route, ":" );
	tokensSize = tokens.size();
	if (tokensSize > 3 || tokensSize < 1)
	{
		Log::Error( "Request route invalid, too colons or empty" );
		this->setError(HTTP_BAD_REQUEST_CODE);
		return ;
	}
	if (tokensSize == 3)
	{
		if (tokens[0] != "http")
		{
			Log::Error( "Request route invalid, when 2 colons must start with http" );
			this->setError(HTTP_BAD_REQUEST_CODE);
			return ;
		}
		if (parseDropHttp(tokens, tokensSize, httpDropped))
			return ;
	}
	if (tokensSize == 2)
	{
		if (httpDropped && tokens[0] == "http")
		{
			Log::Error( "Request route invalid, http repeated" );
			this->setError(HTTP_BAD_REQUEST_CODE);
			return ;
		}
		if (!httpDropped && tokens[0] == "http" && parseDropHttp(tokens, tokensSize, httpDropped))
				return ;
	}
	if (tokensSize == 2) //Stil left 2
	{
		this->routeHost = tokens[0];
		size_t pos = tokens[1].find_first_of('/', 0);
		this->routePort = tokens[1].substr(0, pos);
		this->route = "/";
		if ( pos != std::string::npos )
			this->route = tokens[1].substr( pos );
	}
	if (httpDropped && tokensSize == 1 && tokens[0][0] != '/')
	{
		tokens[0] = '/' + tokens[0];
		this->route = tokens[0];
	}
	return ;
}

bool	Request::checkCgiInRoute( void )
{
	std::string				ext;
	std::string				binary;
	StringVector::iterator	it;
	StringVector::iterator	ite = this->routeChain.end();

	if ( this->routeChain.size() == 0 )
		return ( false );
	for ( it = this->routeChain.begin(); it != ite; it++ )
	{
		if ( getExtensionForPath( *it, ext ) == true )
			binary = this->svr->getCgiBinary( ext, lc );
		this->docExt = ext;
		this->document = *it;
		if ( binary.length() > 0 )
		{
			if ( it + 1 != ite )
				this->pathInfo = "/" + STLUtils::vectorToString< StringVector >( \
					it + 1, ite, "/" );
			this->route = "/" + STLUtils::vectorToString< StringVector >( \
					this->routeChain.begin(), it + 1, "/" );
			this->routeChain.erase( it, ite );
			return ( true );
		}
		binary.clear();
	}
	this->routeChain.erase( this->routeChain.end() - 1 );
	return ( false );
}

void	Request::parseRoute( void )
{
	StringVector::iterator	doc;
	parseQueryStringFromRoute();
	parseHostPortFromRoute();
	this->routeChain = SplitString::split( this->route, "/" );
	if ( this->routeChain.size() == 0 && ( this->route.size() < 1 \
			|| this->route[ 0 ] != '/' ) )
	{
		Log::Error( "routeChain is empty" );
		setError(HTTP_BAD_REQUEST_CODE);
	}
}

void	Request::parseFirstLine( const std::string &line )
{
	StringVector	tokens;

	tokens = SplitString::split( line, " " );
	if ( ( tokens.size() ) < 3 )
	{
		Log::Error( "Request first line incomplete" );
		Log::Error( line );
		Log::Error( "First line tokens nb: " + SUtils::longToString( tokens.size() ) );
		for ( size_t i = 0; i < tokens.size(); i++ )
			Log::Error( "First line token: " + tokens[ i ] );
		setError( HTTP_BAD_REQUEST_CODE );
		return ;
	}
	this->method = SUtils::trim( tokens[ 0 ] );
	this->route = SUtils::trim( tokens[ 1 ] );
	this->protocol = SUtils::trim( tokens[ 2 ] );
	if ( checkProtocol( this->protocol ) == false )
		return ;
	parseRoute();
}

void	Request::parseHeader( const std::string &line )
{
	std::string		key;
	std::string		value;

	if (SplitString::splitHeaderLine(key, value, line))
	{
		this->headers.append(key, value);
		return ;
	}
	setError(400);
}


bool	Request::checkProtocol( std::string protocol )
{
	StringVector	version;

	if ( protocol.length() < 5 || protocol.compare( 0, 5, "HTTP/" ) != 0 \
		|| STLUtils::charCount< std::string >( protocol.begin(), \
			protocol.end(), '.' ) > 1 )
		return ( setError( HTTP_BAD_REQUEST_CODE ) );
	SUtils::split( version, protocol.substr( 5, std::string::npos ), "." );
	if ( version.size() == 0 )
		return ( setError( HTTP_BAD_REQUEST_CODE ) );
	for ( StringVector::iterator it = version.begin(); it != version.end(); it++ )
		if ( SUtils::isNum( *it ) == false || COMPARE_NBR_MAX_STR( *it, short ) > 0 )
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
	if ( SUtils::atoi( version[ 0 ] ) != 1 || ( version.size() == 2 \
		&& SUtils::atoi( version[ 1 ] ) > 1 ) )
		return ( setError( HTTP_VERSION_NOT_SUPPORTED_CODE ) );
	return ( true );
}

bool	Request::checkChunked( void )
{
	Header	*teHead = this->headers.firstWithKey( "Transfer-Encoding" );
	bool	isChunked = false;

	if ( teHead != NULL && ( isChunked = ( teHead->getValue() == "chunked" ) ) )
		this->status = RECVD_CHUNK;
	return ( isChunked );
}

int	Request::splitDocExt( void )
{
	StringVector	frags;
	size_t			len;

	frags = SplitString::split( document, "." );
	len = frags.size();
	if ( len > 1 )
	{
		this->docExt = frags[ len - 1 ];
		return ( 1 );
	}
	return ( 0 );
}

