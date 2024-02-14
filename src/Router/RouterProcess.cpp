/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouterProcess.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 14:11:02 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 11:11:17 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

std::string	Router::methods[ METHODS_NB ] = { "GET", "POST", "PUT", "DELETE", "HEAD" };

bool ( *Router::process[ METHODS_NB ] )( Request& req ) = { &Router::processGetRequest, \
													&Router::processPostRequest, \
													&Router::processPutRequest, \
													&Router::processDeleteRequest, \
													&Router::processHeadRequest };

void	Router::processGetFileRead( Request& req, Client *cli, std::string path)
{
	ssize_t		size;
	int			fd;

	if ( !cli )
		return ;
	Log::Debug( "file to read: " + path );
	size = getFileSize( path );
	if ( size == -1 )
	{
		req.setError( HTTP_INTERNAL_SERVER_ERROR_CODE );
		return ;
	}
	if ( !size )
	{
		cli->setEventWriteSocket();
		return ;
	}
	fd = openReadFile( path );
	if ( cli )
		cli->setEventReadFile( fd );
}

bool	Router::processGetRequest( Request& req )
{
	std::string	path;
	std::string	output;
	int			pathStatus;
	Client		*cli;

	Log::Debug("processGetRequest");
	cli = req.getClient();
	pathStatus = getFileToRead( req, path );
	if ( pathStatus == EXIT_SUCCESS )
		processGetFileRead( req, cli, path );
	else if ( pathStatus == EISDIR && processDirectory( req, path, output ) )
	{
		req.setOutput( output );
		if ( cli )
			cli->setEventWriteSocket();
	}
	return ( req.getError() >= 400 );
}

bool	Router::processHeadRequest( Request& req )
{
	std::string	path;
	std::string	output;
	int			error;
	struct stat	info;
	Client		*cli;

	cli = req.getClient();
	error = getFileToRead( req, path );
	if ( error == EXIT_SUCCESS && !stat( path.c_str(), &info ) )
		req.setOutputLength( info.st_size );
	else if ( error == EISDIR && processDirectory( req, path, output ) )
		req.setOutputLength( output.length() );
	if ( cli )
		cli->enableEventWriteSocket( true );
	return ( req.getError() >= 400 );
}

bool	Router::processPostRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	std::string path;
	Client		*cli;
	int			fd;

	Log::Debug( "processPost" );
	cli = req.getClient();
	if ( !req.isDirectiveSet( "upload_store" ) || document.size() == 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	path = req.getFilePathWrite();
	if ( isDir( path ) )
		return ( req.setError( HTTP_CONFLICT_CODE ) );
	Log::Debug( "isn't a dir" );
	fd = openWriteFile( path );
	if ( fd < 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	Log::Debug( "valid file" );
	if ( cli )
		cli->setEventWriteFile( fd );
	req.setStatus( HTTP_CREATED_CODE );
	return ( false );
}

bool	Router::processPutRequest( Request& req )
{
	std::string	route = req.getRoute();
	std::string bodyContent = req.getBody();
	std::string	document = req.getDocument();
	std::string path;
	Client		*cli;
	int			fd;

	Log::Debug( "processPut" );
	cli = req.getClient();
	if ( !req.isDirectiveSet( "upload_store" ) || document.size() == 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	path = req.getFilePathWrite();
	if ( isDir( path ) )
		return ( req.setError( HTTP_CONFLICT_CODE ) );
	fd = openWriteFile( path );
	if ( fd < 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	if ( cli )
		cli->setEventWriteFile( fd );
	req.setStatus( HTTP_CREATED_CODE );
	return ( false );
}

bool	Router::processDeleteRequest( Request& req )
{
	std::string	path;
	std::string	file;
	Client		*cli;

	cli = req.getClient();
	path = req.getFilePathRead();
	if ( checkPathExist( req, path ) == false )
		return ( false );
	file = path;
	if ( isDir( path ) == true )
		return (req.setError( HTTP_FORBIDDEN_CODE ));
	std::remove(path.c_str());
	req.setStatus( HTTP_NO_CONTENT_CODE );
	if ( cli )
		cli->setEventWriteSocket();
	return ( false );
}

bool	Router::processRequestHeaderReceived( Request &req )
{
	int			i = 0;
	std::string	requestMethod = req.getMethod();
	Client		*cli;

	Log::Debug( "ProcessRequestHeaderReceived" );
	cli = req.getClient();
	if ( cli )
		cli->createResponse();
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
	checkErrorRedir( req.getError(), req );
	checkErrorBody( req, req.getError() );
	return ( true );
}
