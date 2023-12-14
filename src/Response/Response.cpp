/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:02 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/14 13:04:14 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Utils.hpp"
#include "../../inc/Response.hpp"
#include "Response.hpp"

Response::Response() : isCgi(false)
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
	body = b.body;	
	isCgi = b.isCgi;
}

Response&	Response::operator=(const Response& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;	
	isCgi = b.isCgi;
	return (*this);
}

void	 					Response::setServer(std::string server)
{
	this->server = server;
	headers.replace("Server", server);
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

void	 					Response::setHeaders(Headers headers)
{
	this->headers = headers;
}

void	 					Response::setBody(std::string content)
{
	body = content;
	Header h("Content-Lenght", SUtils::longToString(body.length()));
	headers.replace(h);
}

void Response::setIsCgi(bool value)
{
	isCgi = value;
}

void Response::appendHeader(Header header)
{
	headers.append(header);
}

std::string				Response::getServer() const
{
	return (server);
}

std::string				Response::getProtocol() const
{
	return (protocol);
}

std::string				Response::getMethod() const
{
	return (method);
}

std::string				Response::getRoute() const
{
	return (route);
}

unsigned int			Response::getStatus() const
{
	return (status);
}

std::string				Response::getResult() const
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
	(void)OK;
	return (StatusCodes::decode(status));
}

std::string				Response::getQuery() const
{
	return (query);
}

const Headers&			Response::getHeaders() const
{
	return (headers);
}

Header*					Response::getHeader(std::string key)
{
	return (headers.firstWithKey(key));
}

size_t					Response::getContentLength() const
{
	return (body.length());
}

std::string				Response::getBody() const
{
	return (body);
}

bool					Response::getIsCgi() const
{
	return isCgi;
}

std::string				Response::toString()
{
	std::string ret;
	if (!isCgi)
	{
		ret = protocol + " " + SUtils::longToString(status);
		ret += std::string(" ") + getResult() + HEADER_SEP;
		ret += headers.toString();
		ret += HEADER_SEP;
		ret += body;
		return (ret);
	}
	return (body);
}
