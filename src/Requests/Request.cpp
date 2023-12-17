/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 14:45:47 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <Utils.hpp>
#include <SplitString.hpp>
#include <Request.hpp>
#include <Client.hpp>

size_t	Request::id_counter = 0;

Request::Request( void )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->client = NULL;
	this->status = IDLE;
	this->useCgi = false;
	this->error = 0;
	// Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::Request( Client *cli )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->client = cli;
	this->status = FD_BOND;
	this->useCgi = false;
	this->error = 0;
	// Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::~Request( void ) {}

Request::Request( const Request& b )
{
	this->id = Request::id_counter;
	Request::id_counter++;
	this->badRequest = false;
	this->chunkSize = b.chunkSize;
	this->client = b.client;
	this->status = b.status;
	this->method = b.method;
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
	}
	return ( *this );
}

int	Request::bindClient( Client* cli )
{
	this->client = cli;
	status = FD_BOND;
	return ( status );
}

void	Request::parseRoute( void )
{
	StringVector			tokens;
	size_t					len;
	StringVector::iterator	doc;

	tokens = SplitString::split( route, "?" );
	len = tokens.end() - tokens.begin();
	if ( len > 2 )
	{
		Log::Error( "Request query string invalid" );
		this->badRequest = true;
		return ;
	}
	if ( len > 1 )
	{
		this->route = SUtils::trim( tokens[ 0 ] );
		this->query = SUtils::trim( tokens[ 1 ] );                                                                                 
	}
	this->routeChain = SplitString::split( this->route, "/" );
	if ( this->routeChain.size() > 0 && ( this->route.size() > 0 \
			&& this->route[ this->route.size() - 1 ] != '/' ) )
	{
		doc = this->routeChain.end() - 1;
		this->document = *doc;
		this->routeChain.erase( doc );
		splitDocExt();
	}
	if ( this->routeChain.size() == 0 && ( this->route.size() < 1 \
			|| this->route[ 0 ] != '/' ) )
		Log::Error( "routeChain is empty" );
	Log::Info( "Route Chaine: " + getRouteChaineString() );
	Log::Info( "Document: " + getDocument() );
	Log::Info( "Extension: " + getDocExt() );
	//TODO
	//check if route is valid
	//check if route is available
}

void	Request::parseFirstLine( const std::string &line )
{
	StringVector	tokens;

	tokens = SplitString::split( line, " " );
	if ( ( tokens.end() - tokens.begin() ) < 3 )
	{
		Log::Error( "Request first line incomplete" );
		badRequest = true;
		return ;
	}
	this->method = SUtils::trim( tokens[ 0 ] );
	//TODO
	// if (method not in implemented)
	// 	badRequest true;
	this->route = SUtils::trim( tokens[ 1 ] );
	this->protocol = SUtils::trim( tokens[ 2 ] );
	//TODO
	//check if protocol matches
	parseRoute();
}

void	Request::parseHeader( const std::string &line )
{
	StringVector	tokens;

	tokens = SplitString::split( line, ":" );
	if ( ( tokens.end() - tokens.begin() ) < 2 )
	{
		this->headers.append( tokens[ 0 ], "" );
		return ;
	}
	this->headers.append( tokens[ 0 ], SUtils::trim( tokens[ 1 ] ) );	
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

std::string	Request::getHost( void )
{
	Header* h = headers.firstWithKey( "Host" );

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

bool	Request::processLineOnFdBond( const std::string &line )
{
	size_t len = line.length();

	if ( len == 0 || ( len == 1 && line[ 0 ] <= ' ' ) )
	{
		this->status = RECVD_START;
		return ( true );
	}
	parseFirstLine( line );
	this->status = RECVD_REQ_LINE;
	return ( true );
}

bool	Request::processLineOnRecvdStart( const std::string &line )
{
	parseFirstLine( line );
	status = RECVD_REQ_LINE;
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
		if ( checkChunked() )
			return ( true );
		if ( !checkEmptyContent( contentSize ) )
		{
			got = this->client->getNChars(data, contentSize);
			this->body += data;
			if ( got == contentSize )
			{
				this->status = RECVD_ALL;
				// Log::Success(body);
			}
		}
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
		contentSize = std::atol( clHead->getValue().c_str() );
		if ( this->body.size() >= contentSize )
		{
			// Log::Success(body);
			this->status = RECVD_ALL;
		}
		return ( this->client->getPendingSize() > 0 );
	}
	this->status = RECVD_ALL;
	return ( true );
}

bool Request::processLineOnRecvdChunkSize(const std::string &line)
{
	size_t len = line.length();
	// Log::Info("processLineOnRecvdChunkSize with line:");
	// Log::Info(line);
	// Log::Info("chunck size = " + SUtils::longToString(chunkSize));
	// Log::Info("line length = " + SUtils::longToString(len));
	if (chunkSize == 0)
	{
		status = RECVD_LAST_CHUNK;
		return true;
	}	
	if (len == chunkSize)
	{
		body += line;
		status = RECVD_CHUNK;
		return true;
	}
	if ((len == chunkSize + 1) && (line[len - 1] <= ' '))
	{
		body += line.substr(0, len - 1);
		status = RECVD_CHUNK;
		return true;
	}
	return true;
}

bool Request::processLineOnRecvdChunk(const std::string &line)
{
	// Log::Info("processLineOnRecvdChunk with line:");
	// Log::Info(line);
	size_t len = line.length();
	if (((len == 1 || (len == 2 && line[1] <= ' ')) && line[0] == '0' ))
	{
		// Log::Info("Received 0 to indicate last chunk");
		chunkSize = 0;
		status = RECVD_LAST_CHUNK;
		return true;
	}
	chunkSize = atol(line.c_str());
	if (chunkSize > 0)
	{
		// Log::Info("Received" + SUtils::longToString(chunkSize) + "to indicate next chunk size");
		status = RECVD_CHUNK_SIZE;
		return true;
	}
	Log::Info("Received 0 value that indicates last chunk");
	status = RECVD_LAST_CHUNK;
	return true;
}

bool Request::processLineOnRecvdLastChunk(const std::string &line)
{
	// Log::Info("processLineOnRecvdLastChunk with line:");
	// Log::Info(line);
	size_t len = line.length();
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		status = RECVD_ALL;
		// Log::Success(body);
		return true;
	}
	status = RECVD_ALL;
	return true;
}

bool Request::checkChunked()
{
	Header* teHead = headers.firstWithKey("Transfer-Encode");
	bool isChunked = false;
	if (teHead && (isChunked = (teHead->getValue() == "chunked")))
		status = RECVD_CHUNK;
	return isChunked;
}

bool Request::checkKeepAlive()
{
	Header* con = headers.firstWithKey("Connection");
	if (!client)
		return false;
	if (con)
		client->setKeepAlive(con->getValue() == "keep-alive");
	else
		client->setKeepAlive(false);
	return (!!con);
}

int Request::splitDocExt()
{
	std::vector<std::string>	frags;
	frags = SplitString::split(document, ".");
	size_t	len = frags.size();
	if (len > 1)
	{
		docExt = frags[len - 1];
		useCgi = (docExt == std::string("py"));
		return 1;
	}
	return 0;
}

bool Request::checkEmptyContent(size_t& size)
{
	Header* clHead = headers.firstWithKey("Content-Length");
	if (!clHead)
		status = RECVD_ALL;
	else
		size = atol(clHead->getValue().c_str());
	return (status == RECVD_ALL);
}

bool Request::processLine(const std::string &line)
{
	switch (status)
	{
	case IDLE:
		return false;
	case FD_BOND:
		return (processLineOnFdBond(line));
	case RECVD_START:
		return (processLineOnRecvdStart(line));	
	case RECVD_REQ_LINE:
		return (processLineOnRecvdReqLine(line));
	case RECVD_HEADER:
		return (processLineOnRecvdHeader(line));	
	case RECVD_CHUNK_SIZE:
		return (processLineOnRecvdChunkSize(line));
	case RECVD_CHUNK:
		return (processLineOnRecvdChunk(line));
	case RECVD_LAST_CHUNK:
		return (processLineOnRecvdLastChunk(line));
	case RECVD_ALL:
		Log::Info("processLine with RECVD_ALL");
		return false;
	case CGI_LAUNCHED:
		Log::Info("processLine with CGI_LAUNCHED");
		return false;
	case RESP_RENDERED:
		Log::Info("processLine with RESP_RENDERED");
		return false;
	}
	return false;
}

bool								Request::isReadyToSend() const
{
	return (status == RESP_RENDERED);
}

bool Request::isCompleteRecv() const
{
	return (status == RECVD_ALL);
}

bool Request::isCgiLaunched() const
{
	return (status == CGI_LAUNCHED);
}

bool								Request::isReceiving() const
{
	return (status < RECVD_ALL);
}

std::string							Request::toString()
{
	std::string ret = getMethod() + " " + route + " " + protocol + "\n";
	ret += headers.toString();
	ret += HEADER_SEP;
	ret += body;
	return (ret);
}

void								Request::setBody(const std::string& content)
{
	body = content;
}

void Request::setReadyToSend()
{
	if (status == RECVD_ALL || status == CGI_LAUNCHED)
		status = RESP_RENDERED;
}

void Request::setCgiLaunched()
{
	if (status == RECVD_ALL)
		status = CGI_LAUNCHED;
}

void								Request::setCgiOutput(std::string str)
{
	cgiOutput = str;
}

void Request::setUseCgi(bool value)
{
	useCgi = value;
}

void Request::setError(int value)
{
	error = value;
}

void Request::logStatus()
{
	switch (status)
	{
	case IDLE:
		Log::Success("status = IDLE");
		break;
	case FD_BOND:
		Log::Success("status = FD_BOND");
		break;
	case RECVD_START:
		Log::Success("status = RECVD_START");
		break;
	case RECVD_REQ_LINE:
		Log::Success("status = RECVD_REQ_LINE");
		break;
	case RECVD_HEADER:
		Log::Success("status = RECVD_HEADER");
		break;
	case RECVD_CHUNK_SIZE:
		Log::Success("status = RECVD_CHUNK_SIZE");
		break;
	case RECVD_CHUNK:
		Log::Success("status = RECVD_CHUNK");
		break;
	case RECVD_LAST_CHUNK:
		Log::Success("status = RECVD_LAST_CHUNK");
		break;
	case RECVD_ALL:
		Log::Success("status = RECVD_ALL");
		break;
	case CGI_LAUNCHED:
		Log::Success("status = CGI_LAUNCHED");
		break;
	case RESP_RENDERED:
		Log::Success("status = RESP_RENDERED");
		break;
	default:
		Log::Success("status = " + SUtils::longToString(status));
	}
}
