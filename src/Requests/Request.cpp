/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:18:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/11 16:35:55 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <Utils.hpp>
#include <SplitString.hpp>
#include <Request.hpp>
#include <Client.hpp>

size_t	Request::id_counter = 0;

Request::Request(void)
{
	id = id_counter;
	id_counter++;
	client = NULL;
	status = IDLE;
	useCgi = false;
	error = 0;
	// Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::Request(Client *cli)
{
	id = id_counter;
	id_counter++;
	client = cli;
	status = FD_BOND;
	useCgi = false;
	error = 0;
	// Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Request::~Request()
{
}

Request::Request(const Request& b)
{
	id = id_counter;
	id_counter++;
	badRequest = false;
	chunkSize = b.chunkSize;
	client = b.client;
	status = b.status;
	method = b.method;
	route = b.route;
	query = b.query;
	headers = b.headers;
	body = b.body;
	cgiOutput = b.cgiOutput;
	useCgi = b.useCgi;
	error = b.error;
	routeChain = b.routeChain;
	document = b.document;
	docExt = b.docExt;
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
	cgiOutput = b.cgiOutput;
	useCgi = b.useCgi;
	error = b.error;
	routeChain = b.routeChain;
	document = b.document;
	docExt = b.docExt;
	return (*this);
}

int Request::bindClient(Client* cli)
{
	this->client = cli;
	status = FD_BOND;
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
		badRequest = true;
		return ;
	}
	if (len > 1)
	{
		route = SUtils::trim(tokens[0]);
		query = SUtils::trim(tokens[1]);                                                                                 
	}
	routeChain = SplitString::split(route, std::string("/"));
	if (routeChain.size() > 0 && (route.size() > 0 && route[route.size() - 1] != '/'))
	{
		std::vector<std::string>::iterator doc = routeChain.end();
		doc--;
		document = *doc;
		routeChain.erase(doc);
		splitDocExt();
	}
	if (routeChain.size() == 0 && (route.size() < 1 || route[0] != '/'))
		Log::Error("routeChain is empty");
	Log::Info("Route Chaine: " + getRouteChaineString());
	Log::Info("Document: " + getDocument());
	Log::Info("Extension: " + getDocExt());
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
	method = SUtils::trim(tokens[0]);
	//TODO
	// if (method not in implemented)
	// 	badRequest true;
	route = SUtils::trim(tokens[1]);
	protocol = SUtils::trim(tokens[2]);
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

int Request::getError() const
{
	return error;
}

std::string		Request::getCgiOutput() const
{
	return (cgiOutput);
}

bool Request::getUseCgi() const
{
	return useCgi;
}

Client* Request::getClient()
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

std::vector<std::string> Request::getRouteChaine() const
{
	return routeChain;
}

std::string Request::getRouteChaineString() const
{
	size_t it = 0;
	size_t size = routeChain.size();
	std::string chain = std::string("/");
	while (it < size)
	{
		chain += routeChain[it] + "/";
		it++;
	}
	return chain;
}

std::string Request::getDocument() const
{
	return document;
}

std::string Request::getDocExt() const
{
	return docExt;
}

std::string							Request::getQuery() const
{
	return (query);
}

const Headers&						Request::getHeaders() const
{
	return (headers);
}

std::string Request::getHost()
{
	Header* h = headers.firstWithKey("Host");
	if (h)
		return h->getValue();
	else
		return std::string("Unknown");
}

std::string Request::getHeaderWithKey(const std::string& key)
{
	Header* h = headers.firstWithKey(key);
	if (h)
		return h->getValue();
	else
		return std::string("Unknown");
}

size_t								Request::getBodyLength() const
{
	return (body.length());
}

std::string							Request::getBody() const
{
	return (body);
}

size_t Request::getId() const
{
	return id;
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
	size_t contentSize = 0;
	std::string data;
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		status = RECVD_HEADER;
		checkKeepAlive();
		if (checkChunked())
			return true;
		if 	(!checkEmptyContent(contentSize))
		{
			size_t got = client->getNChars(data, contentSize);
			body += data;
			if (got == contentSize)
			{
				status = RECVD_ALL;
				// Log::Success(body);
			}
		}
		return true;
	}
	parseHeader(line);
	return true;
}

bool Request::processLineOnRecvdHeader(const std::string &line)
{
	body += line + '\n';
	Header* clHead = headers.firstWithKey("Content-Length");
	if (clHead)
	{
		size_t contentSize = atol(clHead->getValue().c_str());
		if (body.size() >= contentSize)
		{
			// Log::Success(body);
			status = RECVD_ALL;
		}
		return (client->getPendingSize() > 0);
	}
	status = RECVD_ALL;
	return true;
}

bool Request::processLineOnRecvdChunkSize(const std::string &line)
{
	size_t len = line.length();
	// Log::Info("processLineOnRecvdChunkSize with line:");
	// Log::Info(line);
	// Log::Info("chunck size = " + SUtils::longToString(chunkSize));
	// Log::Info("line length = " + SUtils::longToString(len));
	if (chunkSize == 0)
	{
		status = RECVD_LAST_CHUNK;
		return true;
	}	
	if (len == chunkSize)
	{
		body += line;
		status = RECVD_CHUNK;
		return true;
	}
	if ((len == chunkSize + 1) && (line[len - 1] <= ' '))
	{
		body += line.substr(0, len - 1);
		status = RECVD_CHUNK;
		return true;
	}
	return true;
}

bool Request::processLineOnRecvdChunk(const std::string &line)
{
	// Log::Info("processLineOnRecvdChunk with line:");
	// Log::Info(line);
	size_t len = line.length();
	if (((len == 1 || (len == 2 && line[1] <= ' ')) && line[0] == '0' ))
	{
		// Log::Info("Received 0 to indicate last chunk");
		chunkSize = 0;
		status = RECVD_LAST_CHUNK;
		return true;
	}
	chunkSize = atol(line.c_str());
	if (chunkSize > 0)
	{
		// Log::Info("Received" + SUtils::longToString(chunkSize) + "to indicate next chunk size");
		status = RECVD_CHUNK_SIZE;
		return true;
	}
	Log::Info("Received 0 value that indicates last chunk");
	status = RECVD_LAST_CHUNK;
	return true;
}

bool Request::processLineOnRecvdLastChunk(const std::string &line)
{
	// Log::Info("processLineOnRecvdLastChunk with line:");
	// Log::Info(line);
	size_t len = line.length();
	if (len == 0 || (len == 1 && line[0] <= ' '))
	{
		status = RECVD_ALL;
		// Log::Success(body);
		return true;
	}
	status = RECVD_ALL;
	return true;
}

bool Request::checkChunked()
{
	Header* teHead = headers.firstWithKey("Transfer-Encode");
	bool isChunked = false;
	if (teHead && (isChunked = (teHead->getValue() == "chunked")))
		status = RECVD_CHUNK;
	return isChunked;
}

bool Request::checkKeepAlive()
{
	Header* con = headers.firstWithKey("Connection");
	if (!client)
		return false;
	if (con)
		client->setKeepAlive(con->getValue() == "keep-alive");
	else
		client->setKeepAlive(false);
	return (!!con);
}

int Request::splitDocExt()
{
	std::vector<std::string>	frags;
	frags = SplitString::split(document, ".");
	size_t	len = frags.size();
	if (len > 1)
	{
		docExt = frags[len - 1];
		useCgi = (docExt == std::string("py"));
		return 1;
	}
	return 0;
}

bool Request::checkEmptyContent(size_t& size)
{
	Header* clHead = headers.firstWithKey("Content-Length");
	if (!clHead)
		status = RECVD_ALL;
	else
		size = atol(clHead->getValue().c_str());
	return (status == RECVD_ALL);
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
	case CGI_LAUNCHED:
		Log::Info("processLine with CGI_LAUNCHED");
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

bool Request::isCompleteRecv() const
{
	return (status == RECVD_ALL);
}

bool Request::isCgiLaunched() const
{
	return (status == CGI_LAUNCHED);
}

bool								Request::isReceiving() const
{
	return (status < RECVD_ALL);
}

std::string							Request::toString()
{
	std::string ret = getMethod() + " " + route + " " + protocol + "\n";
	ret += headers.toString();
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
	if (status == RECVD_ALL || status == CGI_LAUNCHED)
		status = RESP_RENDERED;
}

void Request::setReceivedAll()
{
	status = RECVD_ALL;
}

void Request::setCgiLaunched()
{
	if (status == RECVD_ALL)
		status = CGI_LAUNCHED;
}

void								Request::setCgiOutput(std::string str)
{
	cgiOutput = str;
}

void Request::setUseCgi(bool value)
{
	useCgi = value;
}

void Request::setError(int value)
{
	error = value;
}

void Request::logStatus()
{
	switch (status)
	{
	case IDLE:
		Log::Success("status = IDLE");
		break;
	case FD_BOND:
		Log::Success("status = FD_BOND");
		break;
	case RECVD_START:
		Log::Success("status = RECVD_START");
		break;
	case RECVD_REQ_LINE:
		Log::Success("status = RECVD_REQ_LINE");
		break;
	case RECVD_HEADER:
		Log::Success("status = RECVD_HEADER");
		break;
	case RECVD_CHUNK_SIZE:
		Log::Success("status = RECVD_CHUNK_SIZE");
		break;
	case RECVD_CHUNK:
		Log::Success("status = RECVD_CHUNK");
		break;
	case RECVD_LAST_CHUNK:
		Log::Success("status = RECVD_LAST_CHUNK");
		break;
	case RECVD_ALL:
		Log::Success("status = RECVD_ALL");
		break;
	case CGI_LAUNCHED:
		Log::Success("status = CGI_LAUNCHED");
		break;
	case RESP_RENDERED:
		Log::Success("status = RESP_RENDERED");
		break;
	default:
		Log::Success("status = " + SUtils::longToString(status));
	}
}
