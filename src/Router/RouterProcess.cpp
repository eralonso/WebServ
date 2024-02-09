/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouterProcess.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 14:11:02 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/09 17:55:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

std::string	Router::methods[ METHODS_NB ] = { "GET", "POST", "PUT", "DELETE", "HEAD" };

bool ( *Router::process[ METHODS_NB ] )( Request& req ) = { &Router::processGetRequest, \
													&Router::processPostRequest, \
													&Router::processPutRequest, \
													&Router::processDeleteRequest, \
													&Router::processHeadRequest };

bool	Router::processGetRequest( Request& req )
{
	std::string	path;
	std::string	output;
	int			error;
	int			fd;
	Client		*cli;

	Log::Info("processGetRequest");
	cli = req.getClient();
	error = getFileToRead( req, path );
	if ( error == EXIT_SUCCESS )
	{
		Log::Info( "file to read: " + path );
		fd = openReadFile( path );
		if ( cli )
			cli->setEventReadFile( fd );
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
		if ( cli )
		{
			Log::Error( "is dir event write" );
			cli->setEventWriteSocket();
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
	Client		*cli;

	cli = req.getClient();
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

	Log::Info( "processPost" );
	cli = req.getClient();
	if ( !req.isDirectiveSet( "upload_store" ) || document.size() == 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	path = req.getFilePathWrite();
	if ( isDir( path ) )
		return ( req.setError( HTTP_CONFLICT_CODE ) );
	Log::Info( "isn't a dir" );
	fd = openWriteFile( path );
	if ( fd < 0 )
		return ( req.setError( HTTP_FORBIDDEN_CODE ) );
	Log::Info( "valid file" );
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

	Log::Info( "processPut" );
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
	int			error = 0;
	Client		*cli;
	Response	*res;

	Log::Info( "ProcessRequestHeaderReceived" );
	cli = req.getClient();
	if ( cli )
		res = cli->createResponse();
	checkRedir( req );
	if ( req.getError() < MIN_ERROR_CODE )
	{
		if ( req.getUseCgi() )
			return ( processCgi( req ) );
		while ( i < METHODS_NB && Router::methods[ i ] != requestMethod )
			i++;
		if ( i < METHODS_NB )
			error = Router::process[ i ]( req );
		else
			req.setError( HTTP_NOT_ALLOWED_CODE );
	}
	if ( error )
	{
		Log::Error( "Fail request" );
		cli->setEventWriteSocket();
	}
	checkErrorRedir( req.getError(), req );
	checkErrorBody( req, req.getError() );
	return ( true );
}

// bool	Router::processRequestReceived( Request &req )
// {
// 	int			i = 0;
// 	std::string	requestMethod = req.getMethod();

// 	Log::Info( "ProcessRequestReceived" );
// 	checkRedir( req );
// 	if ( req.getError() < MIN_ERROR_CODE )
// 	{
// 		if ( req.getUseCgi() )
// 			return ( processCgi( req ) );
// 		while ( i < METHODS_NB && Router::methods[ i ] != requestMethod )
// 			i++;
// 		if ( i < METHODS_NB )
// 			Router::process[ i ]( req );
// 		else
// 			req.setError( HTTP_NOT_ALLOWED_CODE );
// 	}
// 	checkErrorRedir( req.getError(), req );
// 	checkErrorBody( req, req.getError() );
// 	req.setReadyToSend();
// 	return ( true );
// }
