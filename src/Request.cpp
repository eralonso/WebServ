/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 13:25:25 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../inc/Utils.hpp"
#include "../inc/SplitString.hpp"
#include "../inc/Request.hpp"
#include "Request.hpp"

Request::Request(void)
{
	clientPoll = nullptr;
	status = IDLE;
	parse();
}

Request::Request(struct pollfd *clientPoll)
{
	this->clientPoll = clientPoll;
	status = WAITING_RECV;
}

Request::~Request()
{
}

Request::Request(const Request& b)
{
	clientPoll = clientPoll;
	status = b.status;
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
}

Request&	Request::operator=(const Request& b)
{
	clientPoll = clientPoll;
	status = b.status;
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
	return (*this);
}

int Request::bindClient(struct pollfd *clientPoll)
{
	this->clientPoll = clientPoll;
	status = WAITING_RECV;
	return (status);
}

int Request::appendRecv(const std::string &recv, bool finish)
{
	if (status == WAITING_RECV)
	{
		received += recv;
		if (finish)
			status = RECV_ALL;
	}
	return (status);
}

void Request::parseRoute(void)
{
	std::vector<std::string> tokens = SplitString::split(route,
										std::string("?"));

	size_t	len = tokens.end() - tokens.begin();
	if (len > 2)
	{
		Log::Error("Request query string invalid");
		return ;
	}
	if (len > 1)
	{
		route = tokens[0];
		query = tokens[1];
	}
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
	method = tokens[0];
	route = tokens[1];
	protocol = tokens[2];
	parseRoute();
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

int	Request::parse()
{
	if (status == RECV_ALL)
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
		status = RECV_DECODED;
	}
	return (status);
}

Request::t_status Request::getStatus() const
{
	return (status);
}

struct pollfd* Request::getClientPoll() const
{
	return (clientPoll);
}

std::string Request::getProtocol() const
{
	return (protocol);
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

bool								Request::isReadyToSend() const
{
	return (status == RESP_RENDERED);
}

std::string							Request::toString()
{
	std::string ret = headers.toString();
	ret += HEADER_SEP;
	ret += body;
	return (ret);
}

void								Request::setBody(const std::string& content)
{
	body = content;
	Header h("Content-Lenght", SUtils::longToString(body.length()));
	headers.replace(h);
}

int Request::setDummyRecv()
{
	if (status == WAITING_RECV)
	{
		std::string init("GET / Http/1.1\r\n");
		init += std::string("Host: localhost\r\n");
		init += std::string("Content-Type: text/xml; charset=utf-8\r\n");
		init += std::string("Content-Lenght: 6\r\n");
		init += std::string("Accept-Language: en-us\r\n");
		init += std::string("Accept-Encoding: gzip, deflate\r\n");
		init += std::string("\r\n");
		init += std::string("NoBody\r\n");
		return (appendRecv(init, true));
	}
}
