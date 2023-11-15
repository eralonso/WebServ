/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 13:10:42 by omoreno-         ###   ########.fr       */
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
	body = b.body;
}

Request&	Request::operator=(const Request& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers =  b.headers;
	body = b.body;
	return (*this);
}

void	Request::parse(const std::string& received)
{
	(void)received;
	method = std::string("GET");
	route = std::string("/");
	query = std::string("\"arg1\"=\"value\"");
	body = std::string("{\"data\":\"something\"}");
	headers.append("Content-Lenght", SUtils::longToString(body.length()));
	headers.append("Content-Type", "text/html");
}

 std::string						Request::getMethod() const
{
	return (method);
}

 std::string						Request::getRoute() const
{
	return (route);
}

std::string							Request::getQuery() const
{
	return (query);
}

const Headers&						Request::getHeaders() const
{
	return (headers);
}

size_t								Request::getContentLenght() const
{
	return (body.length());
}

std::string							Request::getBody() const
{
	return (body);
}

std::string							Request::toString()
{
	std::string ret = headers.toString();
	ret += "\r\n";
	ret += body;
	return (ret);
}

void								Request::setBody(const std::string& content)
{
	body = content;
	Header h("Content-Lenght", SUtils::longToString(body.length()));
	headers.replace(h);
}
