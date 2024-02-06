/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/06 13:50:09 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

std::string	Router::methods[ METHODS_NB ] = { "GET", "POST", "PUT", "DELETE", "HEAD" };

bool ( *Router::process[ METHODS_NB ] )( Request& req ) = { &Router::processGetRequest, \
													&Router::processPostRequest, \
													&Router::processPutRequest, \
													&Router::processDeleteRequest, \
													&Router::processHeadRequest };

Router::Router( void ) {}

Router::~Router( void ) {}

Router::Router( const Router& ) {}

Router&	Router::operator=( const Router& )
{
	return ( *this );
}

int	Router::updateResponse( Response &res, Request &req )
{
	res.setServer( SERVER );
	if ( req.getUseCgi() )
		formatCgiResponse( res,req );
	else
		formatGenericResponse( res, req );
	return ( 0 );
}

Response	*Router::getResponse( Request *req )
{
	Response	*res = new Response;

	if ( !req )
		formatErrorResponse( *res, HTTP_INTERNAL_SERVER_ERROR_CODE );
	else
		updateResponse( *res, *req );
	return ( res );
}

std::string	Router::getRequestEmbed( Request &req )
{
	std::string	html;

	html += "\t<h3 style=\"color: #888888;\">Request received in server</h2>\n";
	html += "<p>";
	html += req.toString();
	html += "</p>";
	return ( html );
}

Response	*Router::formatErrorResponse( Response &res, int error )
{
	res.appendHeader( Header( "Content-Type", MimeMap::getMime("html") ) );
	res.setProtocol( "HTTP/1.1" );
	res.setStatus( error );
	res.setBody( "Error: " + SUtils::longToString( error ) );
	return ( &res );
}

bool	Router::processCgi( Request& req )
{
	std::string host;
	std::string port;

	req.getHostPort(host, port);
	try
	{
		CgiExecutor cgiExe(req);

		cgiExe.pushEnvVar("SERVER_SOFTWARE", "webserv");
		cgiExe.pushEnvVar("SERVER_NAME", host);
		cgiExe.pushEnvVar("GATEWAY_INTERFACE", "CGI/1.0");
		cgiExe.pushEnvVar("SERVER_PROTOCOL", req.getProtocol());
		cgiExe.pushEnvVar("SERVER_PORT", port);
		cgiExe.pushEnvVar("REQUEST_METHOD", req.getMethod());
		// cgiExe.pushEnvVar(std::string("FILEPATH_INFO"), req.getRouteChaineString());
		// cgiExe.pushEnvVar("PATH_INFO", req.getRoute());
		cgiExe.pushEnvVar("PATH_INFO", req.getPathInfo() );
		// Log::Error( "get route: " + req.getRoute() );
		// cgiExe.pushEnvVar("PATH_INFO", "../cgi_src/mainTest.tst" );
		// cgiExe.pushEnvVar("PATH_TRANSLATED", "../" + req.getFilePathRead());
		cgiExe.pushEnvVar("SCRIPT_NAME", req.getRoute());
		cgiExe.pushEnvVar("SCRIPT_FILENAME", req.getFilePathRead());
		cgiExe.pushEnvVar("QUERY_STRING", req.getQuery());
		Client *cli = req.getClient();
		if (cli)
		{
			cgiExe.pushEnvVar("REMOTE_HOST", cli->getIpString());
			cgiExe.pushEnvVar("REMOTE_ADDRESS", cli->getIpString());
		}
		else
		{
			cgiExe.pushEnvVar("REMOTE_HOST", "0.0.0.0");
			cgiExe.pushEnvVar("REMOTE_ADDRESS", "0.0.0.0");
		}
		// cgiExe.pushEnvVar("AUTH_TYPE", "none");
		// cgiExe.pushEnvVar("REMOTE_USER", "user");
		// cgiExe.pushEnvVar("REMOTE_IDENT", "user");
		
		cgiExe.pushEnvVar("HTTP_COOKIE", req.getCookies());
		if (req.getBody().size() > 0)
			cgiExe.pushEnvVar("CONTENT_TYPE", req.getHeaderWithKey("Content-Type"));
		cgiExe.pushEnvVar("CONTENT_LENGTH", SUtils::longToString(req.getBody().size()));
		cgiExe.pushEnvVar("HTTP_ACCEPT", req.getHeaderWithKey("Accept"));
		cgiExe.pushEnvVar("USER_AGENT", req.getHeaderWithKey("User-Agent"));
		cgiExe.execute();
		req.setCgiLaunched();
	}
	catch ( const std::exception& e )
	{
		Log::Error ( e.what() );
		req.setError( HTTP_INTERNAL_SERVER_ERROR_CODE );
		checkErrorRedir( req.getError(), req );
		checkErrorBody( req, req.getError() );
		req.setReadyToSend();
	}
	// Once finished CgiTaskPending will send event to change request state to ready to send
	return ( true );
}

bool	Router::processRequestHeaderReceived( Request &req )
{
	int			i = 0;
	std::string	requestMethod = req.getMethod();

	checkRedir( req );
	if ( req.getError() < MIN_ERROR_CODE )
	{
		if ( req.getUseCgi() )
			return ( processCgi( req ) );
		while ( i < METHODS_NB && Router::methods[ i ] != requestMethod )
			i++;
		if ( i < METHODS_NB )
			Router::process[ i ]( req );
		else
			req.setError( HTTP_NOT_ALLOWED_CODE );
	}
	return ( true );
}

//bool	Router::processRequestReceived( Request &req )
//{
//	int			i = 0;
//	std::string	requestMethod = req.getMethod();
//
//	checkRedir( req );
//	if ( req.getError() < MIN_ERROR_CODE )
//	{
//		if ( req.getUseCgi() )
//			return ( processCgi( req ) );
//		while ( i < METHODS_NB && Router::methods[ i ] != requestMethod )
//			i++;
//		if ( i < METHODS_NB )
//			Router::process[ i ]( req );
//		else
//			req.setError( HTTP_NOT_ALLOWED_CODE );
//	}
//	return ( true );
//}

bool	Router::processRequestReceived( Request &req )
{
	int			i = 0;
	std::string	requestMethod = req.getMethod();

	checkRedir( req );
	if ( req.getError() < MIN_ERROR_CODE )
	{
		//req.checkUseCgi();
		if ( req.getUseCgi() )
			return ( processCgi( req ) );
		while ( i < METHODS_NB && Router::methods[ i ] != requestMethod )
			i++;
		if ( i < METHODS_NB )
			Router::process[ i ]( req );
		else
			req.setError( HTTP_NOT_ALLOWED_CODE );
	}
	checkErrorRedir( req.getError(), req );
	checkErrorBody( req, req.getError() );
	req.setReadyToSend();
	return ( true );
}

std::string Router::determineContentType(Response& res, Request& req)
{
	(void)res;
	if ( req.getMethod() == "GET" || req.getMethod() == "HEAD" )
	{
		std::string contentType = MimeMap::getMime(req.getDocExt());
		return (contentType);
	}
	return ( "text/plain" );
}

Response	*Router::formatGenericResponse( Response& res, Request& req )
{
	res.appendHeader( Header( "Content-Type", determineContentType( res, req ) ) );
	res.setProtocol( req.getProtocol() );
	if ( req.getRedir() == true )
		res.appendHeader( Header( "Location", req.getUriRedir() ) );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getMethod() != "HEAD" )
		res.setBody( req.getOutput() );
	else
		res.setBodyLength( req.getOutputLength() );
	return ( &res );
}

bool	Router::parseCgiHeaderLine (Response& res, Request& req, const std::string& line)
{
	std::string	headKey;
	std::string	headValue;

	if (! SplitString::splitHeaderLine(headKey, headValue, line))
		return (false);
	if (headKey == "Status")
	{
		StringVector sv = SplitString::split(headValue, " ");
		size_t svs = sv.size();
		if (svs < 1 || svs > 2)
			return (false);
		std::string statCodeStr = SUtils::trim(sv[0]);
		if (statCodeStr.size() != 3)
			return (false);
		long stat = SUtils::atol(statCodeStr);
		res.setStatus(stat);
		req.setError(stat);
		return (true);
	}
	res.appendHeader(Header(headKey, headValue));
	return (true);
}

bool	Router::parseCgiHeaders (Response& res, Request& req, const std::string& cgiOut)
{
	std::string	body;
	
	res.setIsCgi(false);
	StringVector sv = SplitString::splitHeaderBody(body, cgiOut);
	StringVector::iterator it = sv.begin();
	StringVector::iterator ite = sv.end();
	while (it != ite)
	{
		if (!parseCgiHeaderLine(res, req, *it))
		 	break;
		it++;
	}
	res.setBody(body);
	return (it == ite);
}

bool	Router::parseCgiOutput (Response& res, Request& req)
{
	const std::string& cgiOut = req.getCgiOutput();
	std::string	body;
	bool nph = req.isDocumentNPH ();
	if (nph)
	{
		res.setIsCgi(true);
		res.setBody(cgiOut);
		return ( true );
	}
	return (parseCgiHeaders( res, req, cgiOut ));
}

Response	*Router::formatCgiResponse( Response& res, Request& req )
{
	res.setProtocol( req.getProtocol() );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getError() < MIN_ERROR_CODE )
		parseCgiOutput(res, req);
	else
	{
		res.appendHeader( Header( "Content-Type", "text/html" ) );
		res.setBody( req.getOutput() );
	}
	checkErrorRedir( res.getStatus(), req );
	if ( req.getRedir() == true )
	{
		res.setStatus( req.getError() );
		res.appendHeader( Header( "Location", req.getUriRedir() ) );
	}
	return &res;
}

bool	Router::checkStatMode( std::string path, unsigned int mode )
{
	struct stat	checkMode;

	if ( stat( path.c_str(), &checkMode ) == -1 )
		return ( false );
	return ( ( checkMode.st_mode & mode ) == mode );
}

bool	Router::isDir( std::string path ) { return ( checkStatMode( path, S_IFDIR ) ); }

bool	Router::isFile( std::string path ) { return ( checkStatMode( path, S_IFREG ) ); }

int	Router::openReadFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
		return ( fd );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

int	Router::openWriteFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_WRONLY | O_TRUNC | O_NONBLOCK );
	if ( fd < 0 )
		return ( fd );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

std::string	Router::readFile( std::string file )
{
	std::string		storage;
	char			buffer[ BUFFER_SIZE + 1 ];
	int				readBytes;
	int				fd;

	fd = open( file.c_str(), O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
		return ( "" );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	while ( ( readBytes = read( fd, buffer, BUFFER_SIZE ) ) > 0 )
		storage += std::string( buffer, readBytes );
	close( fd );
	return ( storage );
}

bool	Router::writeFile( std::string file, std::string content )
{
	std::ofstream	outfile;

	outfile.open(file.c_str(), std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
		return ( false );
	outfile.write(content.c_str(), content.size());
	outfile.close();
	return (true);
}

bool	Router::checkPathExist( Request& req, std::string path )
{
	if ( checkStatMode( path, F_OK ) == false )
	{
		req.setError( HTTP_NOT_FOUND_CODE );
		return ( false );
	}
	return ( true );
}

bool	Router::checkPathCanRead( Request& req, std::string path )
{
	if ( checkStatMode( path, R_OK ) == false )
	{
		req.setError( HTTP_FORBIDDEN_CODE );
		return ( false );
	}
	return ( true );
}

bool	Router::isValidDirectory( std::string dir )
{
	return ( dir[ dir.length() -1 ] == '/' );
}

bool	Router::processDirectory( Request& req, std::string path, \
									std::string& output )
{
	if ( req.isAutoindexAllow() == true \
		&& FolderLs::getLs( output, path, req.getRoute() ) == LsEntry::NONE )
		req.setError( HTTP_OK_CODE );
	else
		req.setError( HTTP_FORBIDDEN_CODE );
	return ( req.getError() == HTTP_OK_CODE );
}

void	Router::checkRedir( Request& req )
{
	std::string	uriRedir;
	int			uriCode;

	if ( req.findReturnUri( uriCode, uriRedir ) == true )
		req.setRedirection( uriRedir, uriCode );
}

void 	Router::checkErrorRedir( int errorStatus, Request& req )
{
	bool		redir = false;
	std::string	uriRedir;

	if ( errorStatus >= MIN_ERROR_CODE )
		redir = req.getErrorPage( errorStatus, uriRedir );
	if ( redir == true )
	{
		if ( req.getMethod() == "GET" )
			req.setRedirection( uriRedir, HTTP_MOVED_TEMPORARILY_CODE );
		else
			req.setRedirection( uriRedir, HTTP_SEE_OTHER_CODE );
	}
}

void	Router::checkErrorBody( Request& req, int errorStatus )
{
	if ( errorStatus >= MIN_ERROR_CODE )
	{
		req.setOutput( getDefaultErrorPage( errorStatus ) );
		req.setDocExt( "html" );
	}
}

int	Router::getFileToRead( Request& req, std::string& retFile )
{
	std::string	path;
	std::string	file;

	path = req.getFilePathRead();
	if ( checkPathExist( req, path ) == false )
		return ( ENOENT );
	file = path;
	if ( isDir( path ) == true && req.tryIndexFiles( file ) == false )
	{
		retFile = path;
		return ( EISDIR );
	}
	if ( isValidDirectory( file ) == true )
		return ( ENOENT );
	if ( checkPathCanRead( req, file ) == false )
		return ( EACCES );
	retFile = file;
	req.setError( HTTP_OK_CODE );
	return ( EXIT_SUCCESS );
}

bool	Router::processGetRequest( Request& req )
{
	std::string	path;
	std::string	output;
	int			error;
	int			fd;
	Client		*cli;

	cli = this->request.getClient();
	error = getFileToRead( req, path );
	if ( error == EXIT_SUCCESS )
	{
		fd = openFile( path );
		if ( cli )
			cli->setEventFileRead( fd );
	}
	else if ( error == EISDIR )
	{
		if ( isValidDirectory( path ) == false )
			req.setError( HTTP_NOT_FOUND_CODE );
		else if ( processDirectory( req, path, output ) == true )
		{
			req.setOutput( output );
			req.setDocExt( "html" );
		}
	}
	return ( req.getError() >= 400 );
}

bool	Router::processHeadRequest( Request& req )
{
	std::string	path;
	std::string	output;
	int			error;
	struct stat	info;

	error = getFileToRead( req, path );
	if ( error == EXIT_SUCCESS && !stat( path.c_str(), &info ) )
		req.setOutputLength( info.st_size );
	else if ( error == EISDIR )
	{
		if ( isValidDirectory( path ) == false )
			req.setError( HTTP_NOT_FOUND_CODE );
		else if ( processDirectory( req, path, output ) == true )
		{	
			req.setOutputLength( output.length() );
			req.setDocExt( "html" );
		}
	}
	return ( req.getError() >= 400 );
}

bool	Router::processPostRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	std::string path;

	if ( !req.isDirectiveSet( "upload_store" ) || document.size() == 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	path = req.getFilePathWrite();
	if ( isDir( path ) )
		return ( req.setError( HTTP_CONFLICT_CODE ) );
	if ( !writeFile( path, bodyContent ) )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	return ( req.setError( HTTP_CREATED_CODE ) );
}

bool	Router::processPutRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	std::string path;
	
	if ( !req.isDirectiveSet( "upload_store" ) || document.size() == 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	path = req.getFilePathWrite();
	if ( isDir( path ) )
		return ( req.setError( HTTP_CONFLICT_CODE ) );
	if ( !writeFile( path, bodyContent ) )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	return ( req.setError( HTTP_CREATED_CODE ) );
}

bool	Router::processDeleteRequest( Request& req )
{
	std::string	path;
	std::string	file;

	path = req.getFilePathRead();
	if ( checkPathExist( req, path ) == false )
		return ( false );
	file = path;
	if ( isDir( path ) == true )
		return (req.setError( HTTP_FORBIDDEN_CODE ));
	std::remove(path.c_str());
	req.setError( HTTP_NO_CONTENT_CODE );
	return ( false );
}

std::string	Router::getDefaultErrorPage( unsigned int code )
{
	std::string	def;
	std::string	codeStr;

	codeStr = SUtils::longToString( code ) + " " + StatusCodes::decode( code );
	def += "<html>\n";
	def += "<head><title>" + codeStr + "</title></head>\n";
	def += "<body>\n";
	def += "<center><h1>" + codeStr + "</h1></center>\n";
	def += "<hr><center>webserv/1.0 </center>\n";
	def += "</body>\n";
	def += "</html>\n";
	return ( def );
}
