/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 13:43:13 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "../../inc/Utils.hpp"
#include "../../inc/SplitString.hpp"
#include <Request.hpp>
#include <Client.hpp>

Request::Request(void)
{
	pending = 0;
	client = nullptr;
	status = IDLE;
}

Request::Request(Client *cli)
{
	pending = 0;
	client = cli;
	status = FD_BOND;
}

Request::~Request()
{
}

Request::Request(const Request& b)
{
	badRequest = false;
	chunkSize = b.chunkSize;
	client = b.client;
	status = b.status;
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
}

Request&	Request::operator=(const Request& b)
{
	badRequest = false;
	chunkSize = b.chunkSize;
	client = b.client;
	status = b.status;
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
	return (*this);
}

int Request::bindClient(Client* cli)
{
	this->client = cli;
	status = FD_BOND;
	return (status);
}

bool Request::appendRecv(const std::string &recv)
{
	std::string line;
	received += recv;
	Log::Info("Called appendRecv with:\n" + recv);
	Log::Info("before while in appendRecv");
	logStatus();
	while (!badRequest && getLine(line))
	{
		Log::Info("prev status ");
		logStatus();
		Log::Info("getLine returned:\n" + line);
		bool res = processLine(line);
		Log::Info("resulting in : " + std::string(res ? "true" : "false"));
		logStatus();
	}
	Log::Info("after while in appendRecv isCompleteRecv = " + std::string(isCompleteRecv() ? "true" : "false"));
	return (isCompleteRecv());
}

void Request::parseRoute(void)
{
	std::vector<std::string> tokens = SplitString::split(route,
										std::string("?"));

	size_t	len = tokens.end() - tokens.begin();
	if (len > 2)
	{
		Log::Error("Request query string invalid");
		badRequest = true;
		return ;
	}
	if (len > 1)
	{
		route = tokens[0];
		query = tokens[1];
	}
	//TODO
	//check if route is valid
	//check if route is available
}

void Request::parseFirstLine(const std::string &line)
{
	std::vector<std::string> tokens = SplitString::split(line,
										std::string(" "));
	if ((tokens.end() - tokens.begin()) < 3)
	{
		Log::Error("Request first line incomplete");
		badRequest = true;
		return ;
	}
	method = tokens[0];
	//TODO
	// if (method not in implemented)
	// 	badRequest true;
	route = tokens[1];
	protocol = tokens[2];
	//TODO
	//check if protocol matches
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
	headers.append(tokens[0], SUtils::trim(tokens[1]));	
}

Request::t_status Request::getStatus() const
{
	return (status);
}

Client* Request::getClient() const
{
	return (client);
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

size_t								Request::getBodyLength() const
{
	return (body.length());
}

std::string							Request::getBody() const
{
	return (body);
}

bool Request::isCompleteRecv() const
{
	return (status == RECVD_ALL);
}

bool Request::getLine(std::string& line)
{
	size_t found = received.find('\n', pending);
	if (found == std::string::npos)
		return false;
	line = received.substr(pending, found - pending);
	pending = found + 1;
	return true;
}

bool Request::processLineOnFdBond(const std::string &line)
{
	size_t len = line.length();
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		status = RECVD_START;
		return true;
	}
	parseFirstLine(line);
	status = RECVD_REQ_LINE;
	return true;
}

bool Request::processLineOnRecvdStart(const std::string &line)
{
	parseFirstLine(line);
	status = RECVD_REQ_LINE;
	return true;
}

bool Request::processLineOnRecvdReqLine(const std::string &line)
{
	size_t len = line.length();
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		Log::Info("processLineOnRecvdReqLine detect Header End");
		status = RECVD_HEADER;
		if (received.size() == pending) 
		{
			Log::Info("processLineOnRecvdReqLine set RECVD_ALL");
			status = RECVD_ALL;
		}
		return true;
	}
	parseHeader(line);
	return true;
}

bool Request::processLineOnRecvdHeader(const std::string &line)
{
	Header* clHead = headers.firstWithKey("Content-Length");
	Header* teHead = headers.firstWithKey("Transfer-Encode");
	if (teHead && teHead->getValue() == "chuncked")
	{
		status = RECVD_CHUNK;
		return processLineOnRecvdChunk(line);
	}
	if (clHead)
	{
		size_t size = atoi(clHead->getValue().c_str());
		if (received.size() - pending >= size)
		{
			status = RECVD_ALL;
			return true;
		}
	}
	status = RECVD_ALL;
	Log::Info("processLineOnRecvdHeader set RECVD_ALL cause there's no body");
	return true;
}

bool Request::processLineOnRecvdChunkSize(const std::string &line)
{
	size_t len = line.length();
	if (len == chunkSize)
	{
		body += line;
		return true;
	}
	if ((len == chunkSize + 1) && (line[len - 1] <= ' '))
	{
		body += line.substr(0, len - 1);
		return true;
	}
	return false;
}

bool Request::processLineOnRecvdChunk(const std::string &line)
{
	size_t len = line.length();
	if (((len == 1 || (len == 2 && line[1] <= ' ')) && line[0] == '0' ))
	{
		chunkSize = 0;
		status = RECVD_LAST_CHUNK;
		return true;
	}
	chunkSize = atoi(line.c_str());
	if (chunkSize > 0)
	{
		status = RECVD_CHUNK_SIZE;
		return true;
	}
	return false;
}

bool Request::processLineOnRecvdLastChunk(const std::string &line)
{
	size_t len = line.length();
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		status = RECVD_ALL;
		return true;
	}
	return false;
}

bool Request::processLine(const std::string &line)
{
	switch (status)
	{
	case IDLE:
		return false;
	case FD_BOND:
		return (processLineOnFdBond(line));
	case RECVD_START:
		return (processLineOnRecvdStart(line));	
	case RECVD_REQ_LINE:
		return (processLineOnRecvdReqLine(line));
	case RECVD_HEADER:
		return (processLineOnRecvdHeader(line));	
	case RECVD_CHUNK_SIZE:
		return (processLineOnRecvdChunkSize(line));
	case RECVD_CHUNK:
		return (processLineOnRecvdChunk(line));
	case RECVD_LAST_CHUNK:
		return (processLineOnRecvdLastChunk(line));
	case RECVD_ALL:
		Log::Info("processLine with RECVD_ALL");
		return false;
	case RESP_RENDERED:
		Log::Info("processLine with RESP_RENDERED");
		return false;
	}
	return false;
}

bool								Request::isReadyToSend() const
{
	return (status == RESP_RENDERED);
}


bool								Request::isReceiving() const
{
	return (status < RECVD_ALL);
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
}

void Request::setReadyToSend()
{
	if (status == RECVD_ALL)
		status = RESP_RENDERED;
}

int Request::setDummyRecv()
{
	if (status == FD_BOND)
	{
		std::string init("GET / Http/1.1\r\n");
		init += std::string("Host: localhost\r\n");
		init += std::string("Content-Type: text/xml; charset=utf-8\r\n");
		init += std::string("Content-Lenght: 6\r\n");
		init += std::string("Accept-Language: en-us\r\n");
		init += std::string("Accept-Encoding: gzip, deflate\r\n");
		init += std::string("\r\n");
		init += std::string("NoBody\r\n");
		return (appendRecv(init));
	}
	return (0);
}

void Request::logStatus()
{
	switch (status)
	{
	case IDLE:
		Log::Info("status = IDLE");
		break;
	case FD_BOND:
		Log::Info("status = FD_BOND");
		break;
	case RECVD_START:
		Log::Info("status = RECVD_START");
		break;
	case RECVD_REQ_LINE:
		Log::Info("status = RECVD_REQ_LINE");
		break;
	case RECVD_HEADER:
		Log::Info("status = RECVD_HEADER");
		break;
	case RECVD_CHUNK_SIZE:
		Log::Info("status = RECVD_CHUNK_SIZE");
		break;
	case RECVD_CHUNK:
		Log::Info("status = RECVD_CHUNK");
		break;
	case RECVD_LAST_CHUNK:
		Log::Info("status = RECVD_LAST_CHUNK");
		break;
	case RECVD_ALL:
		Log::Info("status = RECVD_ALL");
		break;
	case RESP_RENDERED:
		Log::Info("status = RESP_RENDERED");
		break;
	default:
		Log::Info("status = " + SUtils::longToString(status));
	}
}