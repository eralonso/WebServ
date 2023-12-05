/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/05 17:20:44 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include "../../inc/Response.hpp"
#include "../../inc/Receptionist.hpp"
#include "Client.hpp"
#include "Router.hpp"

size_t	Client::id_counter = 0;

Client::Client(void)
{
	id = id_counter;
	id_counter++;
	keepAlive = false;
	pending = 0;
	socket = -1;
	Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Client::Client(socket_t pollsocket, WSPoll& polls)
{
	id = id_counter;
	id_counter++;
	keepAlive = false;
	pending = 0;
	socket = pollsocket;
	this->polls = &polls;
	Log::Info("Created request id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Client::~Client()
{
}

Client::Client(const Client& b)
{
	id = id_counter;
	id_counter++;
	socket = b.socket;
	pending = b.pending;
	received = b.received;
}

Client&	Client::operator=(const Client& b)
{
	socket = b.socket;
	pending = b.pending;
	received = b.received;
	return (*this);
}

int Client::bindClientPoll(socket_t pollsocket)
{
	socket = pollsocket;
	return 0;
}

socket_t	Client::getClientSocket()
{
	return (socket);
}

size_t	Client::getId()
{
	return id;
}

void	Client::LogId()
{
	Log::Info("Client id: " + SUtils::longToString((long)id));
}

Request*	Client::findRecvRequest()
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
	bool		cont = true;
	while (cont && !fail && getLine(line))
	{
		Request* req = findRecvRequest();
		if (req)
			cont = req->processLine(line);
		else
		{
			req = appendRequest(this);
			if (req)
				cont = req->processLine(line);
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
		if (Router::processRequestReceived(*req))
			count++;
	}
	return (count);
}

int	Client::managePollout()
{
	Request* req;
	Response* res;
	int count = 0;
	while ((req = findReadyToSendRequest()))
	{
		res = Router::getResponse(req);
		if (res && sendResponse(res->toString()))
		{
			count++;
			eraseRequest();
			delete res;
			res = nullptr;
		}
	}
	return count;
}

bool Client::getKeepAlive() const
{
	return keepAlive;
}

int	Client::sendResponse(std::string resp)
{
	if (socket >= 0)
		return (Receptionist::sendResponse(socket, resp));
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

size_t Client::getNChars(std::string &data, size_t n)
{
	size_t remain = received.size() - pending;
	if (n > remain)
		n = remain;
	data = received.substr(pending, n);
	pending += n;
	return n;
}

size_t	Client::getPendingSize() const
{
	return (received.size() - pending);
}

// int Client::setDummyRecv()
// {
// 	std::string init("GET / Http/1.1\r\n");
// 	init += std::string("Host: localhost\r\n");
// 	init += std::string("Content-Type: text/xml; charset=utf-8\r\n");
// 	init += std::string("Content-Lenght: 6\r\n");
// 	init += std::string("Accept-Language: en-us\r\n");
// 	init += std::string("Accept-Encoding: gzip, deflate\r\n");
// 	init += std::string("\r\n");
// 	init += std::string("NoBody\r\n");
// 	received += init;
// 	return (1);
// }

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

void Client::allowPollWrite(bool value)
{
	polls->allowPollWrite(socket, value);
}

bool Client::checkPendingToSend()
{
	LogId();
	if (Requests::checkPendingToSend())
	{
		allowPollWrite(true);
		return true;
	}
	return false;
}
