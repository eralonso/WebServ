/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/28 15:23:12 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include "../../inc/Response.hpp"
#include "../../inc/Receptionist.hpp"
#include "Client.hpp"

Client::Client(void)
{
	this->clientPoll = nullptr;
	keepAlive = false;
	pending = 0;
}

Client::Client(struct pollfd *cliPoll)
{
	this->clientPoll = cliPoll;
	keepAlive = false;
	pending = 0;
}


Client::~Client()
{
}

int Client::bindClientPoll(pollfd *cliPoll)
{
	this->clientPoll = cliPoll;
	return 0;
}

Client::Client(const Client& b)
{
	clientPoll = b.clientPoll;
	pending = b.pending;
	received = b.received;
}

Client&	Client::operator=(const Client& b)
{
	clientPoll = b.clientPoll;
	pending = b.pending;
	received = b.received;
	return (*this);
}

struct pollfd *Client::getClientPoll()
{
	return (this->clientPoll);
}

Request* Client::findRecvRequest()
{
	Request* req;
	Requests::iterator	it = begin();
	Requests::iterator	ite = end();
	while (it != ite)
	{
		req = *it;
		if(req && req->isReceiving())
			return (req);
		it++;
	}
	return nullptr;
}

Request* Client::findCompleteRecvRequest()
{
	Request* req;
	Requests::iterator	it = begin();
	Requests::iterator	ite = end();
	while (it != ite)
	{
		ite--;
		req = *ite;
		if(req && req->isCompleteRecv())
			return (req);
	}
	return nullptr;
}

Request* Client::findReadyToSendRequest()
{
	Request* req;
	Requests::iterator	ite = end();
	if (size() > 0)
	{
		ite--;
		req = *ite;
		if(req && req->isReadyToSend())
			return (req);
	}
	return nullptr;
}

int Client::manageRecv(std::string recv)
{
	received += recv;
	std::string line;
	bool		fail = false;
	while (!fail && getLine(line))
	{
		Request* req = findRecvRequest();
		if (req)
			req->processLine(line);
		else
		{
			req = appendRequest(this);
			if (req)
				req->processLine(line);
			else
				fail = true;
		}
	}
	purgeUsedRecv();
	if (fail)
	{
		Log::Error("Coudn't create new Request");
		return 1;
	}
	return 0;
}

int	Client::manageCompleteRecv()
{
	Request* req;
	int count = 0;
	while ((req = findCompleteRecvRequest()))
	{
		req->setReadyToSend();
		count++;
	}
	return (count);
}

int	Client::managePollout()
{
	Request* req;
	int count = 0;
	while ((req = findReadyToSendRequest()))
		if (sendResponse(getResponse(req)))
		{
			count++;
			eraseRequest();
		}
	return count;
}

std::string	Client::getHtml( void )
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
	html += "\n";
	html += "</body>\n";
	html += "</html>";
	return ( html );
}

std::string Client::getResponse(Request *req)
{
	Response res;
	res.setServer("OREginx");
	res.appendHeader(Header("Content-Type", std::string("text/html")));
	if (!req)
	{
		res.setProtocol("HTTP/1.1");
		res.setStatus(500);
		res.setBody("Error: 500");
	}
	else
	{
		res.setProtocol(req->getProtocol());
		res.setStatus(200);
		res.setMethod(req->getMethod());
		res.setBody(getHtml());
	}
	return res.toString();
}

bool Client::getKeepAlive() const
{
	return keepAlive;
}

int	Client::sendResponse(std::string resp)
{
	if (clientPoll)
		return (Receptionist::sendResponse(clientPoll->fd, resp));
	return (0);
}

bool Client::getLine(std::string& line)
{
	size_t found = received.find('\n', pending);
	if (found == std::string::npos)
		return false;
	line = received.substr(pending, found - pending);
	pending = found + 1;
	return true;
}

size_t	Client::getPendingSize() const
{
	return (received.size() - pending);
}

int Client::setDummyRecv()
{
	std::string init("GET / Http/1.1\r\n");
	init += std::string("Host: localhost\r\n");
	init += std::string("Content-Type: text/xml; charset=utf-8\r\n");
	init += std::string("Content-Lenght: 6\r\n");
	init += std::string("Accept-Language: en-us\r\n");
	init += std::string("Accept-Encoding: gzip, deflate\r\n");
	init += std::string("\r\n");
	init += std::string("NoBody\r\n");
	received += init;
	return (1);
}

bool Client::setKeepAlive(bool value)
{
	keepAlive = value;
	return keepAlive;
}

size_t Client::purgeUsedRecv()
{
	size_t pendSize = getPendingSize();
	received = received.substr(pending, pendSize);
	pending = 0;
	return (pendSize);
}

