/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/28 11:03:34 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <Router.hpp>
#include <CgiExecutor.hpp>
#include <FolderLs.hpp>

Router::Router( void ) {}

Router::~Router( void ) {}

int	Router::updateResponse( Response &res, Request &req )
{
	res.setServer( req.getHost() );
	if ( req.getDocument() == "favicon.ico" )
		createFaviconRes( res, req );
	else if ( req.getUseCgi() && req.getError() == 0 )
		formatCgiResponse( res,req );
	else if ( req.getError() != 0 )
	{
		Log::Info( "updateResponse detect error status: " \
			+ SUtils::longToString( req.getError() ) );
		if ( req.getError() == 100 )
			formatContinueResponse( res, req );
		else if ( req.getError() == 202 )
			formatAcceptResponse( res, req );
		else
			formatErrorResponse( res, req );
	}
	else if ( req.getMethod() == "GET" )
		formatGenericResponse( res, req );
	else
		formatAcceptResponse( res, req );
	return ( 0 );
}

std::string	Router::getHtml( Request *req )
{
	std::string		html;
	std::string		resFolderLs;
	std::string		readBuf;
	std::string		route = req->getRoute();
	std::ifstream	infile;
	std::string		path = "." + route;

	Log::Info("Path to GET ... " + path);
	if (access(path.c_str(), R_OK) == 0)
	{
		std::string dirPath(path);
		if (dirPath.size() == 0 || (*(dirPath.end() - 1)) != '/')
			dirPath += '/';
		if (FolderLs::getLs(resFolderLs, dirPath, route) == FolderLs::CANTOPENDIR)
		{
			infile.open(path.c_str(), std::ios::in); 	
			if (infile.is_open())
			{
				while (!std::getline(infile, readBuf).eof())
					html += readBuf + std::string("\n");
				Log::Info("Read ... \n" + html);
				infile.close();
				return ( html );
			}
		}
		return (resFolderLs);
	}
	html = "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "\t<meta charset=\"UTF-8\">\n";
	html += "\t<title>ª</title>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "\t<h1 style=\"color: #00FFFF;\">Message from server</h1>\n";
	if ( req != NULL )
		html += getRequestEmbed( *req );
	html += "\n";
	html += getForm();
	html += "</body>\n";
	html += "</html>";
	return ( html );
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
		html += "<h3style=\"color: #FF0000;\">Error: ";
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
	int			error;

	if ( !res )
		return ( res );
	if ( !req )
		formatErrorResponse( *res, 500 );
	else if ( ( error = req->getError() ) )
	{
		if ( error == 100 )
			formatContinueResponse( *res, *req );
		if ( error == 202 )
			formatAcceptResponse( *res, *req );
		else
			formatErrorResponse( *res, error );
	}
	else
		updateResponse( *res, *req );
	return ( res );
}

Response	*Router::createFaviconRes( Response& res, Request& req )
{
	std::string html;

	res.setProtocol( req.getProtocol() );
	res.setStatus( 200 );
	res.setMethod( req.getMethod() );
	res.appendHeader( Header( "Content-Type", "image/svg+xml" ) );
	html += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"150\" height=\"100\" viewBox=\"0 0 3 2\">\n";
	html += "<rect width=\"1\" height=\"2\" x=\"0\" fill=\"#008d46\" />\n";
	html += "<rect width=\"1\" height=\"2\" x=\"1\" fill=\"#ffffff\" />\n";
	html += "<rect width=\"1\" height=\"2\" x=\"2\" fill=\"#d2232c\" />\n";
	html += "<circle cx=\"1\" cy=\"1\" r=\".5\" fill=\"#0000ff\" />\n";
	html += "</svg>\n";
	res.setBody( html );
	return ( &res );
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
	res.appendHeader( Header( "Content-Type", "text/html" ) );
	res.setProtocol( "HTTP/1.1" );
	res.setStatus( error );
	res.setBody( "Error: " + SUtils::longToString( error ) );
	return ( &res );
}

bool	Router::processRequestReceived( Request &req )
{
	Log::Success("Router::processRequestReceived");
	std::string script = "";
	std::string host = "";
	std::string port = "";
	req.getHostPort(host, port);
	req.checkUseCgi();
	if ( !req.getUseCgi() )
	{
		req.setReadyToSend();
		return ( true );
	}
	try
	{

		CgiExecutor cgiExe(req);
		cgiExe.pushEnvVar(std::string("SERVER_SOFTWARE"), "webserv");
		cgiExe.pushEnvVar(std::string("SERVER_NAME"), host);
		cgiExe.pushEnvVar(std::string("GATEWAY_INTERFACE"), "CGI/1.0");
		cgiExe.pushEnvVar(std::string("SERVER_PROTOCOL"), req.getProtocol());
		cgiExe.pushEnvVar(std::string("SERVER_PORT"), port);
		cgiExe.pushEnvVar(std::string("REQUEST_METHOD"), req.getMethod());
		cgiExe.pushEnvVar(std::string("PATH_INFO"), req.getRouteChaineString());
		cgiExe.pushEnvVar(std::string("PATH_TRANSLATED"), req.getRouteChaineString());
		cgiExe.pushEnvVar(std::string("SCRIPT_NAME"), req.getDocument());
		cgiExe.pushEnvVar(std::string("QUERY_STRING"), req.getQuery());
		Client *cli = req.getClient();
		if (cli)
		{
			cgiExe.pushEnvVar(std::string("REMOTE_HOST"), cli->getIpString());
			cgiExe.pushEnvVar(std::string("REMOTE_ADDRESS"), cli->getIpString());
		}
		else
		{
			cgiExe.pushEnvVar(std::string("REMOTE_HOST"), "0.0.0.0");
			cgiExe.pushEnvVar(std::string("REMOTE_ADDRESS"), "0.0.0.0");
		}
		cgiExe.pushEnvVar(std::string("AUTH_TYPE"), "none");
		cgiExe.pushEnvVar(std::string("REMOTE_USER"), "user");
		cgiExe.pushEnvVar(std::string("REMOTE_IDENT"), "user");
		if (req.getBody().size() > 0)
			cgiExe.pushEnvVar(std::string("CONTENT_TYPE"), req.getHeaderWithKey("Content-Type"));
		cgiExe.pushEnvVar(std::string("CONTENT_LENGTH"), SUtils::longToString(req.getBody().size()));
		cgiExe.pushEnvVar(std::string("HTTP_ACCEPT"), req.getHeaderWithKey("Accept"));
		cgiExe.pushEnvVar(std::string("USER_AGENT"), req.getHeaderWithKey("User-Agent"));
		cgiExe.execute();
		req.setCgiLaunched();
		return ( true );
	}
	catch ( const std::exception& e )
	{
		Log::Error ( "When trying to execute CGI" );
		Log::Error ( e.what() );
		// TODO Set Error to Send in request so the proper response is formed to send
		req.setError( 500 );
		req.setReadyToSend();
		return ( true );
	}
	// Once finished CgiTaskPending will send event to change request state to ready to send
	return ( false );
}

std::string Router::determineContentType(Response& res, Request& req)
{
	(void)res;
	std::string ext(req.getDocExt());
	if (ext == std::string("png") || ext == std::string("PNG"))
		return std::string("image/png");
	if (ext == std::string("jpg") || ext == std::string("JPG"))
		return std::string("image/jpg");
	return std::string("text/html");
}

Response*	Router::formatGenericResponse( Response& res, Request& req )
{
	res.appendHeader(Header("Content-Type", determineContentType(res, req)));
	res.setProtocol(req.getProtocol());
	int errorStatus = req.getError();
	if (errorStatus == 0)
		errorStatus = 200;
	res.setStatus( errorStatus );
	res.setMethod( req.getMethod() );
	res.setBody( getHtml( &req ) );
	return ( &res );
}

Response	*Router::formatCgiResponse( Response& res, Request& req )
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol(req.getProtocol());
	res.setStatus(200);
	res.setMethod(req.getMethod());
	res.setBody(req.getCgiOutput());
	std::string doc = req.getDocument();
	res.setBody(req.getCgiOutput());
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
	res.setStatus(202);
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
