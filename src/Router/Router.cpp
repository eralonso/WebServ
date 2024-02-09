/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/09 18:40:46 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

Router::Router( void ) {}

Router::~Router( void ) {}

Router::Router( const Router& ) {}

Router&	Router::operator=( const Router& )
{
	return ( *this );
}

int	Router::updateResponse( Response &res, Request &req, Client& cli )
{
	Log::Info("updateResponse");
	res.setServer( SERVER );
	if ( req.getUseCgi() )
		formatCgiResponse( res,req, cli );
	else
		formatGenericResponse( res, req );
	res.updateResString();
	return ( 0 );
}

// Response	*Router::getResponse( Request *req )
// {
// 	Response	*res = new Response;

// 	if ( !req )
// 		formatErrorResponse( *res, HTTP_INTERNAL_SERVER_ERROR_CODE );
// 	else
// 		updateResponse( *res, *req );
// 	return ( res );
// }

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
	Log::Info("formatGenericResponse");
	res.appendHeader( Header( "Content-Type", determineContentType( res, req ) ) );
	res.setProtocol( req.getProtocol() );
	if ( req.getRedir() == true )
		res.appendHeader( Header( "Location", req.getUriRedir() ) );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getMethod() != "HEAD" )
	{
		if ( req.getOutput().length() > 0 )
			res.setBody( req.getOutput() );
	}
	else
		res.setBodyLength( req.getOutputLength() );
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

int	Router::openReadFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_RDONLY | O_NONBLOCK );
	if ( fd < 0 )
		return ( fd );
	Log::Success( "Open file read: " + SUtils::longToString( fd ) );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

int	Router::openWriteFile( std::string file )
{
	int	fd;

	fd = open( file.c_str(), O_WRONLY | O_TRUNC | O_NONBLOCK | O_CREAT, 0666 );
	if ( fd < 0 )
		return ( fd );
	Log::Success( "Open file write: " + SUtils::longToString( fd ) );
	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
	return ( fd );
}

// std::string	Router::readFile( std::string file )
// {
// 	std::string		storage;
// 	char			buffer[ BUFFER_SIZE + 1 ];
// 	int				readBytes;
// 	int				fd;

// 	fd = open( file.c_str(), O_RDONLY | O_NONBLOCK );
// 	if ( fd < 0 )
// 		return ( "" );
// 	fcntl( fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC );
// 	while ( ( readBytes = read( fd, buffer, BUFFER_SIZE ) ) > 0 )
// 		storage += std::string( buffer, readBytes );
// 	close( fd );
// 	return ( storage );
// }

// bool	Router::writeFile( std::string file, std::string content )
// {
// 	std::ofstream	outfile;

// 	outfile.open(file.c_str(), std::ios::out | std::ios::trunc);
// 	if (!outfile.is_open())
// 		return ( false );
// 	outfile.write(content.c_str(), content.size());
// 	outfile.close();
// 	return (true);
// }

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
		req.setStatus( HTTP_OK_CODE );
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
	req.setStatus( HTTP_OK_CODE );
	return ( EXIT_SUCCESS );
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
