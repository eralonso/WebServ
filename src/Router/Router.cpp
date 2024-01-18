/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/16 12:36:51 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <Router.hpp>
#include <CgiExecutor.hpp>
#include <FolderLs.hpp>
#include <MimeMap.hpp>

std::string	Router::methods[ METHODS_NB ] = { "GET", "POST", "PUT", "DELETE", "HEAD" };

bool ( *Router::process[ METHODS_NB ] )( Request& req ) = { &Router::processGetRequest, \
													&Router::processPostRequest, \
													&Router::processPutRequest, \
													&Router::processDeleteRequest, \
													&Router::processHeadRequest };

Router::Router( void ) {}

Router::~Router( void ) {}

int	Router::updateResponse( Response &res, Request &req )
{
	Log::Error( "Uppdating response" );
	res.setServer( req.getHost() );
	if ( req.getUseCgi() )
		formatCgiResponse( res,req );
	else
		formatGenericResponse( res, req );
	/*else if ( req.getError() != 0 )
	{
		Log::Info( "updateResponse detect error status: " \
			+ SUtils::longToString( req.getError() ) );
		if ( req.getError() == 100 )
			formatContinueResponse( res, req );
		else if ( req.getError() == HTTP_ACCEPTED_CODE )
			formatAcceptResponse( res, req );
		else
			formatErrorResponse( res, req );
	}
	else
		formatAcceptResponse( res, req );
	*/
	return ( 0 );
}

std::string	Router::getHtml( Request *req )
{
	std::string		html;
	std::string		resFolderLs;
	// std::string		readBuf;
	std::string		route = req->getRoute();
	// std::ifstream	infile;
	std::string		path = req->getFilePath();

	Log::Info("Path to GET ... " + path);
	if (access(path.c_str(), R_OK) == 0)
	{
		std::string dirPath(path);
		if (dirPath.size() == 0 || (*(dirPath.end() - 1)) != '/')
			dirPath += '/';
		if (FolderLs::getLs(resFolderLs, dirPath, route) == FolderLs::CANTOPENDIR)
			return (readFile(path));
		// {
		// 	infile.open(path.c_str(), std::ios::in);
		// 	if (infile.is_open())
		// 	{
		// 		while (!std::getline(infile, readBuf).eof())
		// 			html += readBuf + std::string("\n");
		// 		Log::Info("Read ... \n" + html);
		// 		infile.close();
		// 		return ( html );
		// 	}
		// }
		return (resFolderLs);
	}
	// html = "<!DOCTYPE html>\n";
	// html += "<html lang=\"en\">\n";
	// html += "<head>\n";
	// html += "\t<meta charset=\"UTF-8\">\n";
	// html += "\t<title>ª</title>\n";
	// html += "</head>\n";
	// html += "<body>\n";
	// html += "\t<h1 style=\"color: #00FFFF;\">Message from server</h1>\n";
	// if ( req != NULL )
	// 	html += getRequestEmbed( *req );
	// html += "\n";
	// html += getForm();
	// html += "</body>\n";
	// html += "</html>";
	// return ( html );
	if (req->getDocument() == "favicon.ico")
	{
		path = "./favicon.png";
		if (access(path.c_str(), R_OK) == 0)
				return (readFile(path));
	}
	req->setError(HTTP_NOT_FOUND_CODE);
	return(std::string(""));
}

std::string	Router::getHtmlErrorPage( Request *req )
{
	std::string	html;

	html = "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "\t<meta charset=\"UTF-8\">\n";
	html += "\t<title>ª</title>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "\t<h1 style=\"color: #FF2222;\">Error Message from server</h1>\n";
	if ( req != NULL )
	{
		html += "<h3 style=\"color: #FF0000;\">Error: ";
		html += SUtils::longToString( req->getError() ) + "</h3>\n\n";
		html += getRequestEmbed( *req );
	}
	html += "</body>\n";
	html += "</html>";
	return ( html );
}

std::string	Router::getForm( void )
{
	std::string form = std::string( "<form method=\"POST\">\n" );

	form += "<input id=\"firstname\" name=\"firstname\"/>\n";
	form += "<input id=\"surname\" name=\"surname\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	form += "<form method=\"POST\">\n";
	form += "<input type=\"file\" id=\"filename\" name=\"filename\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	form += "<form method=\"POST\">\n";
	form += "<input type=\"file\" id=\"filename\" name=\"filename\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	return ( form );
}

Response	*Router::getResponse( Request *req )
{
	Response	*res = new Response;
	//int			error;

	if ( !req )
		formatErrorResponse( *res, HTTP_INTERNAL_SERVER_ERROR_CODE );
	else
		updateResponse( *res, *req );
	//else if ( ( error = req->getError() ) )
	//{
	//	if ( error == 100 )
	//		formatContinueResponse( *res, *req );
	//	if ( error == HTTP_ACCEPTED_CODE )
	//		formatAcceptResponse( *res, *req );
	//	else
	//		formatErrorResponse( *res, error );
	//}
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
		cgiExe.pushEnvVar("PATH_INFO", ".");
		cgiExe.pushEnvVar("PATH_TRANSLATED", req.getDocument());
		// cgiExe.pushEnvVar(std::string("SCRIPT_NAME"), req.getDocument());
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
		cgiExe.pushEnvVar("AUTH_TYPE", "none");
		cgiExe.pushEnvVar("REMOTE_USER", "user");
		cgiExe.pushEnvVar("REMOTE_IDENT", "user");
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
		Log::Error ( "When trying to execute CGI" );
		Log::Error ( e.what() );
		// TODO Set Error to Send in request so the proper response is formed to send
		req.setError( HTTP_INTERNAL_SERVER_ERROR_CODE );
		req.setReadyToSend();
	}
	// Once finished CgiTaskPending will send event to change request state to ready to send
	return ( true );
}

bool	Router::processRequestReceived( Request &req )
{
	int			i = 0;
	std::string	requestMethod = req.getMethod();

	// Log::Success("Router::processRequestReceived");
	checkRedir( req );
	if ( req.getError() < MIN_ERROR_CODE )
	{
		req.checkUseCgi();
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
	if (req.getMethod() == "GET")
	{
		std::string contentType = MimeMap::getMime(req.getDocExt());
		return (contentType);
	}
	return (std::string("text/html"));
}

Response	*Router::formatGenericResponse( Response& res, Request& req )
{
	Log::Error( "Generic response" );
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

Response	*Router::formatCgiResponse( Response& res, Request& req )
{
	Log::Error( "Cgi response" );
	res.appendHeader( Header( "Content-Type", "text/html" ) );
	res.setProtocol( req.getProtocol() );
	//res.setStatus( HTTP_OK_CODE );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getError() < MIN_ERROR_CODE )
		res.setBody( req.getCgiOutput() );
	else
		res.setBody( req.getOutput() );
	std::string doc = req.getDocument();
	bool nph = (doc.size() > 2
		&& (doc[0] == 'n' || doc[0] == 'N')
		&& (doc[1] == 'p' || doc[1] == 'P')
		&& (doc[2] == 'h' || doc[2] == 'H'));
	res.setIsCgi(nph);
	return &res;
}

Response *Router::formatContinueResponse(Response& res, Request& req)
{
	Log::Info("formatContinueResponse");
	res.appendHeader(Header("Accept", req.getHeaderWithKey("Content-Type")));
	res.setProtocol(req.getProtocol());
	res.setStatus(req.getError());
	res.setMethod(req.getMethod());
	return &res;
}

Response *Router::formatAcceptResponse(Response& res, Request& req)
{
	Log::Info("formatAcceptResponse");
	res.setProtocol(req.getProtocol());
	res.setStatus(HTTP_ACCEPTED_CODE);
	res.setMethod(req.getMethod());
	return &res;
}

Response	*Router::formatErrorResponse( Response& res, Request& req )
{
	res.appendHeader( Header( "Content-Type", "text/html" ) );
	res.setProtocol( req.getProtocol() );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	res.setBody( getHtml( &req ) );
	return ( &res );
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

std::string	Router::readFile( std::string file )
{
	std::string		storage;
	std::string		buffer;
	std::ifstream	fd;

	fd.open( file.c_str() );
	if ( fd.is_open() == false )
		return ( "" );
	while ( !std::getline( fd, buffer ).eof() )
		storage += buffer + "\n";
	fd.close();
	return ( storage );
}

bool	Router::writeFile( std::string file, std::string content )
{
	std::ofstream	outfile;

	outfile.open(file.c_str(), std::ios::out | std::ios::trunc); 	
	if (!outfile.is_open())
		return ( false );
	outfile.write(content.c_str(), content.size());
	//Log::Info("Written ... \n" + content);
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
	Log::Info( "path to get directory: " + path );
	if ( req.isAutoindexAllow() == true \
		&& FolderLs::getLs( output, path, req.getRoute() ) == FolderLs::NONE )
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
		req.setOutput( getDefaultErrorPage( errorStatus ) );
}

int	Router::getFileToRead( Request& req, std::string& retFile )
{
	std::string	path;
	std::string	file;

	path = req.getFilePath();
	if ( checkPathExist( req, path ) == false )
		return ( ENOENT );
	file = path;
		Log::Info( "path to get directory: " + path );
	if ( isDir( path ) == true && req.tryIndexFiles( file ) == false )
	{
		retFile = path;
		return ( EISDIR );
	}
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

	Log::Info( "path to get file: " + req.getFilePath() );
	error = getFileToRead( req, path );
	if ( error == EXIT_SUCCESS )
		req.setOutput( readFile( path ) );
	else if ( error == EISDIR )
	{
		if ( isValidDirectory( path ) == false )
			req.setError( HTTP_NOT_FOUND_CODE );
		else if ( processDirectory( req, path, output ) == true )
			req.setOutput( output );
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
			req.setOutputLength( output.length() );
	}
	return ( req.getError() >= 400 );
}

bool	Router::processPostRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	// std::ofstream	outfile;
	std::string path;
	
	if (bodyContent.size() == 0)
		return (req.setError(HTTP_NO_CONTENT_CODE) ); //Status No Content
	if (!req.isDirectiveSet( "upload_store" ) || document.size() == 0)
		return ( req.setError(HTTP_FORBIDDEN_CODE) ); //Forbidden
	path = req.getFilePath();
	Log::Info("Path to POST ... " + path);
	if (! writeFile(path, bodyContent))
		return ( req.setError(HTTP_FORBIDDEN_CODE) ); //Forbidden
	return ( req.setError( HTTP_CREATED_CODE ) ); //Created
}

bool	Router::processPutRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	// std::ofstream	outfile;
	std::string path;
	
	if (bodyContent.size() == 0)
		return (req.setError(HTTP_NO_CONTENT_CODE) ); //Status No Content
	if (!req.isDirectiveSet( "upload_store" ) || document.size() == 0)
		return ( req.setError(HTTP_FORBIDDEN_CODE) ); //Forbidden
	path = req.getFilePath();
	Log::Info("Path to POST ... " + path);
	if (! writeFile(path, bodyContent))
		return ( req.setError(HTTP_FORBIDDEN_CODE) ); //Forbidden
	return ( req.setError( HTTP_CREATED_CODE ) ); //Created
}

bool	Router::processDeleteRequest( Request& req )
{
	std::string	path;
	std::string	file;

	path = req.getFilePath();
	if ( checkPathExist( req, path ) == false )
	{
		req.setError(HTTP_NOT_FOUND_CODE);
		return ( false );
	}
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
