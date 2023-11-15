/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 15:54:00 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <string>
#include <Utils.hpp>
#include <SplitString.hpp>

Request::Request(void)
{
	std::string init("GET / Http 1.1\r\n");
	init += std::string("Content-Type: text/html\r\n");
	init += std::string("Content-Lenght: 6\r\n");
	init += std::string("\r\n");
	init += std::string("NoBody\r\n");
	parse(init);
}

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
	headers = b.headers;
	body = b.body;
}

Request&	Request::operator=(const Request& b)
{
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
	return (*this);
}

void Request::parseFirstLine(const std::string &line)
{
	std::vector<std::string> tokens = SplitString::split(line,
										std::string(" "));
	if ((tokens.end() - tokens.begin()) < 3)
	{
		Log::Error("Request first line incomplete");
		return ;
	}
}

void Request::parseHeader(const std::string &line)
{
	std::vector<std::string> tokens = SplitString::split(line,
										std::string(":"));	
	if ((tokens.end() - tokens.begin()) < 2)
	{
		headers.append(tokens[0], std::string(""));	
		return ;
	}
	headers.append(tokens[0], tokens[1]);	
}

void Request::parseHead(const std::string &head)
{
	// method = std::string("GET");
	// route = std::string("/");
	// query = std::string("\"arg1\"=\"value\"");
	// body = std::string("{\"data\":\"something\"}");
	// headers.append("Content-Lenght", SUtils::longToString(body.length()));
	// headers.append("Content-Type", "text/html");
	std::vector<std::string> lines = SplitString::split(head,
										std::string(HEADER_SEP));
	std::vector<std::string>::iterator itb = lines.begin();
	std::vector<std::string>::iterator it = itb;
	std::vector<std::string>::iterator ite = lines.end();
	while (it != ite)
	{
		if (it == itb)
			parseFirstLine(*it);
		else
			parseHeader(*it);
		it++;
	}
}

void	Request::parse(const std::string& received)
{
	std::string sep(HEADER_SEP);
	sep += sep;
	std::string head;
	size_t loc = received.find(sep);
	if (loc != std::string::npos)
	{
		head = received.substr(0 ,loc);
		size_t beginBody = loc + sep.length();
		setBody(received.substr(beginBody ,received.length() - beginBody));
	}
	else
	{
		head = received;
		setBody(std::string(""));
	}
	parseHead(head);
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
