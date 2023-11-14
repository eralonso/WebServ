/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:02 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:19:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Response.hpp>
#include <Utils.hpp>

Response::Response()
{
}

Response::~Response()
{
}

Response::Response(const Response& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	contentTypes = b.contentTypes;
	contentLength = b.contentLength;
	body = b.body;	
}

Response&	Response::operator=(const Response& b)
{
	method = b.method;
	route = route;
	query = query;
	headers = headers;
	contentTypes = contentTypes;
	contentLength = contentLength;
	body = body;	
	return (*this);
}

void	 					Response::setServer(std::string server)
{
	this->server = server;
}

void	 					Response::setProtocol(std::string protocol)
{
	this->protocol = protocol;
}

void	 					Response::setMethod(std::string method)
{
	this->method = method;
}

void	 					Response::setRoute(std::string route)
{
	this->route = route;
}

void	 					Response::setStatus(unsigned int status)
{
	this->status = status;
}

void	 					Response::setQuery(std::string query)
{
	this->query = query;
}

void	 					Response::setHeaders(std::vector<std::string> headers)
{
	this->headers = headers;
}

void	 					Response::setContentTypes(std::vector<std::string> contentTypes)
{
	this->contentTypes = contentTypes;
}

void						Response::setContentLength(size_t size)
{
	this->contentLength = size;
}

void	 					Response::setBody(std::string body)
{
	this->body = body;
	this->contentLength = body.length();
}

void	 					Response::pushHeader(std::string header)
{
	headers.push_back(header);
}

void	 					Response::pushContentType(std::string contentType)
{
	contentTypes.push_back(contentType);
}

const std::string				Response::getServer() const
{
	return (server);
}

const std::string				Response::getProtocol() const
{
	return (protocol);
}

const std::string				Response::getMethod() const
{
	return (method);
}

const std::string				Response::getRoute() const
{
	return (route);
}

unsigned int					Response::getStatus() const
{
	return (status);
}

const std::string				Response::getResult() const
{
	/* Informational responses (100 – 199)
	Successful responses (200 – 299)
	Redirection messages (300 – 399)
	Client error responses (400 – 499)
	Server error responses (500 – 599)*/
	int hundreds = ((status)/100 % 10);
	bool informational = hundreds == 1;
	bool successful = hundreds == 2;
	bool redirection = hundreds == 3;
	bool OK = (informational || successful || redirection);
	return (std::string(OK ? "OK" : "KO"));
}

const std::string				Response::getQuery() const
{
	return (query);
}

const std::vector<std::string>	Response::getHeaders() const
{
	return (headers);
}

const std::vector<std::string>	Response::getContentTypes() const
{
	return (contentTypes);
}

const size_t					Response::getContentLength() const
{
	return (contentLength);
}

const std::string				Response::getBody() const
{
	return (body);
}

const std::string				Response::contentTypesToString() const
{
	std::string ret;
	ret = std::string("Content-Type:");
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite;
	while (it != ite)
	{
		ret += *it;
		it++;
		if (it != ite)
			ret += ",";
	}
	ret += "\r\n";
	return (ret);
}

const std::string				Response::headersToString() const
{
	std::string ret;
	ret = std::string("Content-Type:");
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite;
	while (it != ite)
	{
		ret += *it;
		it++;
		if (it != ite)
			ret += ",";
	}
	ret += "\r\n";
	return (ret);
}

const std::string				Response::toString() const
{
	std::string ret;
	ret = protocol + " " + SUtils::longToString(status);
	ret += std::string(" ") + getResult() + "\r\n";
	ret += "Server: " + server + "\r\n";
	ret += "Content-Length: ";
	ret += SUtils::longToString(body.length());
	ret += "\r\n";
	ret += contentTypesToString();
	ret += "\r\n";
	ret += body;
	return (ret);
}
