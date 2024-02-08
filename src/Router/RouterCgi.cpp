/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouterCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 14:04:22 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 14:06:58 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

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
		//cgiExe.pushEnvVar("CONTENT_LENGTH", SUtils::longToString(req.getBody().size()));
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

bool	Router::parseCgiOutput (Response& res, Request& req, Client& cli)
{
	const std::string& cgiOut = cli.getCgiOutput();
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

Response	*Router::formatCgiResponse( Response& res, Request& req, Client& cli )
{
	res.setProtocol( req.getProtocol() );
	res.setStatus( req.getError() );
	res.setMethod( req.getMethod() );
	if ( req.getError() < MIN_ERROR_CODE )
		parseCgiOutput(res, req, cli);
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
