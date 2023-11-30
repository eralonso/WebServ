/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:17 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 14:38:52 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Router.hpp>

Router::Router(/* args */)
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
	else
		formatGenericResponse(res, req);	
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

std::string Router::getForm(void)
{
	std::string	form = std::string("<form method=\"POST\">\n");
	form += "<input id=\"firstname\" name=\"firstname\"/>\n";
	form += "<input id=\"surname\" name=\"surname\"/>\n";
	form += "<input type=\"submit\"/>\n";
	form += "</form>\n";
	return form;
}

Response* Router::getResponse(Request* req)
{
	Response* res = new Response;
	if (!res)
		return res;
	if (!req)
		formatErrorResponse(*res);
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

Response *Router::formatErrorResponse(Response &res)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol("HTTP/1.1");
	res.setStatus(500);
	res.setBody("Error: 500");	
	return &res;
}

bool Router::isRequestForCgi(Request &req)
{
	(void)req;
	return false;
}

bool Router::processRequestReceived(Request &req)
{
	if (!isRequestForCgi(req))
		return true;
	// TODO
	// Delegate result formation to Ggi.
	// Once it is finished send event to change request state to ready to send
	return true;
}

Response *Router::formatGenericResponse(Response& res, Request& req)
{
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	res.setProtocol(req.getProtocol());
	res.setStatus(200);
	res.setMethod(req.getMethod());
	res.setBody(getHtml(&req));	
	return &res;
}
