/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/12 17:48:52 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>
#include <CgiExecutor.hpp>

Router::Router()
{
}

Router::~Router()
{
}

int Router::updateResponse(Response &res, Request &req)
{
	res.setServer(req.getHost());
	if (req.getDocument()==std::string("favicon.ico"))
		createFaviconRes(res, req);
	// else if (req.getDocExt() == std::string("py"))
	else if (req.getUseCgi() && req.getError() == 0)
		formatCgiResponse(res,req);
	else if (req.getError() != 0)
	{
		Log::Info("updateResponse detect error status: " + SUtils::longToString(req.getError()));
		if (req.getError() == 100)
			formatContinueResponse(res, req);
		else if (req.getError() == 202)
			formatAcceptResponse(res, req);
		else
			formatErrorResponse(res, req);
	}
	else if (req.getMethod() == "GET")
		formatGenericResponse(res, req);	
	else
		formatAcceptResponse(res, req);
	return 0;
}

std::string	Router::getHtml(Request* req)
{
	std::string	html;
	html = "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "\t<meta charset=\"UTF-8\">\n";
	html += "\t<title>ª</title>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "\t<h1 style=\"color: #00FFFF;\">Message from server</h1>\n";
	if (req)
		html += getRequestEmbed(*req);
	html += "\n";
	html += getForm();
	html += "</body>\n";
	html += "</html>";
	return ( html );
}

std::string	Router::getHtmlErrorPage(Request* req)
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
	if (req)
	{

		html += "<h3style=\"color: #FF0000;\">Error: ";
		html += SUtils::longToString(req->getError())+ "</h3>\n\n";
		html += getRequestEmbed(*req);
	}
	html += "</body>\n";
	html += "</html>";
	return ( html );
}

std::string Router::getForm(void)
{
	std::string	form = std::string("<form method=\"POST\">\n");
	form += "<input id=\"firstname\" name=\"firstname\"/>\n";
	form += "<input id=\"surname\" name=\"surname\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	form += "<form method=\"POST\">\n";
	form += "<input type=\"file\" id=\"filename\" name=\"filename\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	return form;
}

Response* Router::getResponse(Request* req)
{
	Response* res = new Response;
	int error;
	if (!res)
		return res;
	if (!req)
		formatErrorResponse(*res, 500);
	else if ((error = req->getError()))
	{
		if (error == 100)
			formatContinueResponse(*res, *req);
		if (error == 202)
			formatAcceptResponse(*res, *req);
		else
			formatErrorResponse(*res, error);
	}
	else
		updateResponse(*res, *req);
	return res;
}

Response *Router::createFaviconRes(Response& res, Request& req)
{
	res.setProtocol(req.getProtocol());
	res.setStatus(200);
	res.setMethod(req.getMethod());
	std::string html;
	res.appendHeader(Header("Content-Type", "image/svg+xml"));
	html += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"150\" height=\"100\" viewBox=\"0 0 3 2\">\n";
	html += "<rect width=\"1\" height=\"2\" x=\"0\" fill=\"#008d46\" />\n";
	html += "<rect width=\"1\" height=\"2\" x=\"1\" fill=\"#ffffff\" />\n";
	html += "<rect width=\"1\" height=\"2\" x=\"2\" fill=\"#d2232c\" />\n";
	html += "<circle cx=\"1\" cy=\"1\" r=\".5\" fill=\"#0000ff\" />\n";
	html += "</svg>\n";
	res.setBody(html);
	return &res;
}

std::string Router::getRequestEmbed(Request &req)
{
	std::string html;
	html += "\t<h3 style=\"color: #888888;\">Request received in server</h2>\n";
	html += "<p>";
	html += req.toString();
	html += "</p>";
	return html;
}

Response *Router::formatErrorResponse(Response &res, int error)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol("HTTP/1.1");
	res.setStatus(error);
	res.setBody("Error: " + SUtils::longToString(error));	
	return &res;
}

bool Router::processRequestReceived(Request &req)
{
	if (!req.getUseCgi())
	{
		req.setReadyToSend();
		return true;
	}
	try
	{

		std::string script = "";
		CgiExecutor cgiExe(req, nullptr);
		cgiExe.pushEnvVar(std::string("ROUTE"), req.getRouteChaineString());
		cgiExe.pushEnvVar(std::string("PATH"), req.getRoute());
		cgiExe.pushEnvVar(std::string("QUERY_STRING"), req.getQuery());
		cgiExe.pushEnvVar(std::string("HOST"), req.getHost());
		cgiExe.pushEnvVar(std::string("METHOD"), req.getMethod());
		cgiExe.execute();
		req.setCgiLaunched();
		return true;
	}
	catch(const std::exception& e)
	{
		Log::Error ("When trying to execute CGI");
		Log::Error (e.what());
		// TODO Set Error to Send in request so the proper response is formed to send
		req.setError(500);
		req.setReadyToSend();
		return true;
	}
	// Once finished CgiTaskPending will send event to change request state to ready to send
	return false;
}

Response *Router::formatGenericResponse(Response& res, Request& req)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol(req.getProtocol());
	int errorStatus = req.getError();
	if (errorStatus == 0)
		errorStatus = 200;
	res.setStatus(errorStatus);
	res.setMethod(req.getMethod());
	res.setBody(getHtml(&req));	
	return &res;
}

Response *Router::formatCgiResponse(Response& res, Request& req)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol(req.getProtocol());
	res.setStatus(200);
	res.setMethod(req.getMethod());
	res.setBody(req.getCgiOutput());
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

Response *Router::formatErrorResponse(Response& res, Request& req)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol(req.getProtocol());
	res.setStatus(req.getError());
	res.setMethod(req.getMethod());
	res.setBody(getHtml(&req));	
	return &res;
}
