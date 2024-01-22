/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/18 18:18:19 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <Utils.hpp>
#include <SplitString.hpp>
#include <Request.hpp>
#include <Client.hpp>
#include <ServerFinder.hpp>
#include <CgiExecutor.hpp>

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
	// Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::~Request( void ) 
{
	CgiExecutor::purgeDiscardedRequest(this);
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
	}
	return ( *this );
}

int	Request::bindClient( Client* cli )
{
	this->client = cli;
	this->status = FD_BOND;
	return ( this->status );
}

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

bool	Request::getExtensionForPath( std::string path, std::string& ext )
{
	size_t	pos;

	pos = path.find_last_of( "." );
	if ( pos == std::string::npos )
		return ( false );
	ext = path.substr( pos + 1 );	
	return ( true );
}

#include <Utils.hpp>

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
			Log::Info( "Is a cgi" );
			return ( true );
		}
		binary.clear();
	}
	this->routeChain.erase( this->routeChain.end() - 1 );
	Log::Info( "docExt: " + this->docExt + " && document: " + this->document );
	Log::Info( "routeChain: " + STLUtils::vectorToString< StringVector >( \
		this->routeChain.begin(), this->routeChain.end(), "/" ) );
	Log::Info( "Isn't a cgi" );
	return ( false );
}

void	Request::parseRoute( void )
{
	StringVector::iterator	doc;
	parseQueryStringFromRoute();
	parseHostPortFromRoute();
	this->routeChain = SplitString::split( this->route, "/" );
	//if ( this->routeChain.size() > 0 && ( this->route.size() > 0 
	//		&& this->route[ this->route.size() - 1 ] != '/' ) )
	//{
	//	doc = this->routeChain.end() - 1;
	//	this->document = *doc;
	//	this->routeChain.erase( doc );
	//	splitDocExt();
	//}
	if ( this->routeChain.size() == 0 && ( this->route.size() < 1 \
			|| this->route[ 0 ] != '/' ) )
	{
		Log::Error( "routeChain is empty" );
		setError(HTTP_BAD_REQUEST_CODE);
	}
	// Log::Info( "Host: " + routeHost);
	// Log::Info( "Port: " + routePort);
	// Log::Info( "Route Chaine: " + getRouteChaineString() );
	// Log::Info( "Document: " + getDocument() );
	// Log::Info( "Extension: " + getDocExt() );
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
	size_t		contentSize = 0;
	std::string	data;
	size_t		got;

	if ( len == 0 || ( len == 1 && line[ 0 ] <= ' ' ) )
	{
		this->status = RECVD_HEADER;
		checkKeepAlive();
		if (!updateServerConfig())
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		updateLocation();
		this->useCgi = checkCgiInRoute();
		maxBodySize = svr->getMaxBodySize(getRoute());
		if (svr->getIsAllowedMethod( this->lc, this->method ) == false)
			return ( setError( HTTP_NOT_ALLOWED_CODE ) );
		updateFilePaths();
		if ( checkChunked() )
			return ( true );
		if ( !checkEmptyContent( contentSize ) && (maxBodySize == 0 || contentSize <= maxBodySize) )
		{
			got = this->client->getNChars(data, contentSize);
			this->body += data;
			if ( got == contentSize )
				this->status = RECVD_ALL;
		}
		if (maxBodySize != 0 && contentSize > maxBodySize)
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		return ( true );
	}
	parseHeader( line );
	return ( true );
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
		if ( this->body.size() >= contentSize )
			this->status = RECVD_ALL;
		return ( this->client->getPendingSize() > 0 );
	}
	this->status = RECVD_ALL;
	return ( true );
}

bool	Request::processLineOnRecvdChunkSize( const std::string &line )
{
	size_t len = line.length();

	if ( this->chunkSize == 0 )
	{
		this->status = RECVD_LAST_CHUNK;
		return ( true );
	}	
	if ( len == this->chunkSize )
	{
		this->body += line;
		if (this->body.size() > this->maxBodySize && this->maxBodySize != 0)
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		this->status = RECVD_CHUNK;
		return ( true );
	}
	if ( ( len == this->chunkSize + 1 ) && ( line[ len - 1 ] <= ' ' ) )
	{
		this->body += line.substr( 0, len - 1 );
		if (this->body.size() > this->maxBodySize && this->maxBodySize != 0)
			return ( setError( HTTP_BAD_REQUEST_CODE ) );
		this->status = RECVD_CHUNK;
		return ( true );
	}
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
	this->chunkSize = SUtils::atol( line.c_str() );
	if ( chunkSize > 0 )
	{
		this->status = RECVD_CHUNK_SIZE;
		return ( true );
	}
	Log::Info( "Received 0 value that indicates last chunk" );
	this->status = RECVD_LAST_CHUNK;
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
	this->status = RECVD_ALL;
	return ( true );
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
			Log::Info( "processLine with RECVD_ALL" );
			return ( false );
		case CGI_LAUNCHED:
			Log::Info( "processLine with CGI_LAUNCHED" );
			return ( false );
		case RESP_RENDERED:
			Log::Info( "processLine with RESP_RENDERED" );
			return ( false );
	}
	return ( false );
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
	Header	*teHead = this->headers.firstWithKey( "Transfer-Encode" );
	bool	isChunked = false;

	if ( teHead != NULL && ( isChunked = ( teHead->getValue() == "chunked" ) ) )
		this->status = RECVD_CHUNK;
	return ( isChunked );
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

//void	Request::checkUseCgi( void )
//{
//	std::string	binary;
//
//	this->useCgi = false;
//	if ( this->cgiDocExt.size() == 0 )
//		return ;
//	binary = this->svr->getCgiBinary( this->cgiDocExt, lc );
//	this->useCgi = ( binary.size() > 0 );
//}

bool	Request::checkEmptyContent( size_t& size )
{
	Header	*clHead = headers.firstWithKey( "Content-Length" );

	if ( !clHead )
		this->status = RECVD_ALL;
	else
		size = SUtils::atol( clHead->getValue().c_str() );
	return ( this->status == RECVD_ALL );
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

bool	Request::updateServerConfig( void )
{
	this->svr = ServerFinder::find( *this );
	if ( this->svr != NULL )
		return ( true );
	return ( false );
}

void	Request::updateLocation( void )
{
	//std::string	routeWithoutFile = getRouteChaineString();
	std::string	routeWithoutFile = getRoute();

	Log::Error( "location string: " + routeWithoutFile );
	if ( this->svr != NULL )
		this->lc = this->svr->getLocationAtPath( routeWithoutFile );
}

void	Request::updateFilePaths( void )
{
	std::string	routeWithoutFile = getRoute();

	if ( this->svr == NULL )
	{
		this->filePathWrite = ConfigUtils::pathJoin( ".", this->route );
		this->filePathRead = ConfigUtils::pathJoin( ".", this->route );
		return ;
	}
	// if ( this->method == "POST" || this->method == "PUT" )
	// {
	// 	return ;
	// }
	this->filePathWrite = svr->getFinalUploadPath( routeWithoutFile, this->lc );
	this->filePathRead = svr->getFinalPath( routeWithoutFile, this->lc );
	Log::Success( "filePathRead == " + this->filePathRead );
	Log::Success( "filePathWrite == " + this->filePathWrite );
}

void Request::setDefaultFavicon(void)
{
	this->route = "/favicon.svg";
	parseRoute();
	setError(0);
	updateFilePaths();
}

bool	Request::tryIndexFiles( std::string& file ) const
{
	if ( this->svr != NULL )
		return ( this->svr->tryIndexFiles( file, this->filePathRead, this->lc ) );
	return ( false );
}

bool	Request::findReturnUri( int& uriCode, std::string& uriRedirection ) const
{
	if ( this->svr != NULL )
		return ( this->svr->findReturnUri( uriCode, uriRedirection, this->lc ) );
	return ( false );
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

std::string	Request::getBody( void ) const
{
	return ( this->body );
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

std::string	Request::toString( void )
{
	std::string ret = getMethod() + " " + this->route + " " + this->protocol + "\n";

	ret += this->headers.toString();
	ret += HEADER_SEP;
	ret += this->body;
	return ( ret );
}

void	Request::setBody( const std::string& content )
{
	this->body = content;
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
	if ( value >= HTTP_BAD_REQUEST_CODE )
		this->badRequest = true;
	this->status = RECVD_ALL;
	this->error = value;
	return ( this->badRequest != true );
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
	setError( code );
}

void	Request::setOutputLength( size_t size )
{
	this->outputLength = size;
}

void	Request::logStatus( void )
{
	switch ( status )
	{
		case IDLE:
			Log::Success( "status = IDLE" );
			break ;
		case FD_BOND:
			Log::Success( "status = FD_BOND" );
			break ;
		case RECVD_START:
			Log::Success( "status = RECVD_START" );
			break ;
		case RECVD_REQ_LINE:
			Log::Success( "status = RECVD_REQ_LINE" );
			break ;
		case RECVD_HEADER:
			Log::Success( "status = RECVD_HEADER" );
			break ;
		case RECVD_CHUNK_SIZE:
			Log::Success( "status = RECVD_CHUNK_SIZE" );
			break ;
		case RECVD_CHUNK:
			Log::Success( "status = RECVD_CHUNK" );
			break ;
		case RECVD_LAST_CHUNK:
			Log::Success( "status = RECVD_LAST_CHUNK" );
			break ;
		case RECVD_ALL:
			Log::Success( "status = RECVD_ALL" );
			break ;
		case CGI_LAUNCHED:
			Log::Success( "status = CGI_LAUNCHED" );
			break ;
		case RESP_RENDERED:
			Log::Success( "status = RESP_RENDERED" );
			break ;
		default:
			Log::Success( "status = " + SUtils::longToString( status ) );
	}
}
std::string Request::getCookies( void ) const
{
	return (headers.getCookies());	
}
