/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:28:19 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <string>
#include <Utils.hpp>

Request::Request(const std::string&	received)
{
	parse(received);
}

Request::~Request()
{
}

Request::Request(const Request& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers =  b.headers;
	contentTypes = b.contentTypes;
	contentLength = b.contentLength;
	body = b.body;
}

Request&	Request::operator=(const Request& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers =  b.headers;
	contentTypes = b.contentTypes;
	contentLength = b.contentLength;
	body = b.body;
	return (*this);
}

void	Request::parse(const std::string& received)
{
	(void)received;
	method = std::string("GET");
	route = std::string("/");
	query = std::string("\"arg1\"=\"value\"");
	contentTypes.push_back(std::string("text/html"));
	body = std::string("{\"data\":\"something\"}");
}

const std::string&					Request::getMethod() const
{
	return (method);
}

const std::string&					Request::getRoute() const
{
	return (route);
}

const std::string&					Request::getQuery() const
{
	return (query);
}

const std::map<std::string, std::string>&	Request::getHeaders() const
{
	return (headers);
}

const std::vector<std::string>&	Request::getContentTypes() const
{
	return (contentTypes);
}

size_t								Request::getContentLenght() const
{
	return (contentLength);
}

const std::string&					Request::getBody() const
{
	return (body);
}

const std::string					Request::contentTypesToString() const
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

std::string							Request::toString() const
{
	std::string ret;
	ret += "Content-Length: ";
	ret += SUtils::longToString(body.length());
	ret += "\r\n";
	ret += contentTypesToString();
	ret += "\r\n";
	ret += body;
	return (ret);
}
