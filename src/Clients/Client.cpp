/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/11 12:44:53 by omoreno-         ###   ########.fr       */
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
	this->polls = nullptr;
	// Log::Info("Created client id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
}

Client::Client(socket_t pollsocket, WSPoll& polls)
{
	id = id_counter;
	id_counter++;
	keepAlive = false;
	pending = 0;
	socket = pollsocket;
	this->polls = &polls;
	// Log::Info("Created client id: " + SUtils::longToString(id) + " & address " + SUtils::longToString((long)this));
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
	Request* req;
	received += recv;
	std::string line;
	bool		fail = false;
	bool		cont = true;
	while (cont && !fail && getLine(line))
	{
		req = findRecvRequest();
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
	size_t pend = getPendingSize();
	if (fail)
	{
		Log::Error("Coudn't create new Request");
		return 1;
	}
	if (req)
	{
		Log::Info("Must Continue 100?");
		std::string cs = req->getHeaderWithKey(std::string("content-length"));
		Log::Info("Content-Length: " + cs);
		Log::Info("pend: " + SUtils::longToString(pend));
		Log::Info("BodyLen: " + SUtils::longToString((long)req->getBodyLength()));
		if (pend == 0 && req->isReceiving() && 
			cs != "unknown" && (long)req->getBodyLength() < atol(cs.c_str()))
			{
				Log::Info("Continue 100");
				req->setError(100);
				req->setReceivedAll();
				req->getClient()->setKeepAlive(true);
			}
	}
	return 0;
}

int	Client::manageCompleteRecv()
{
	Request* req;
	int count = 0;
	while ((req = findCompleteRecvRequest()))
	{
		Log::Info("Req. Completed Recv: " + SUtils::longToString(req->getId()));
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
	Log::Info("managePollout: ");
	while ((req = findReadyToSendRequest()))
	{
		res = Router::getResponse(req);
		Log::Info("managePollout for: " + SUtils::longToString(req->getId()));
		Log::Info("Response: \n" + res->toString());
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
	if (keepAlive)
		Log::Info("getKeepAlive: true");
	else
		Log::Info("getKeepAlive: false");
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
	
	if (value)
		Log::Info("setKeepAlive: true");
	else
		Log::Info("setKeepAlive: false");
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
	struct pollfd	*clientPoll;	
	if (polls)
	{
		// polls->allowPollWrite(socket, value);
		try
		{
			clientPoll = &(polls->operator[](socket));
		}
		catch ( std::out_of_range& e )
		{ 
			Log::Info( "ClientPoll for [ " \
				+ SUtils::longToString( (long)socket )\
				+ " ]: " \
				+ "not found");
			return ;
		}
		if (value)
			clientPoll->events |= POLLOUT;
		else 
			clientPoll->events &= ~POLLOUT;
	// if (polls)
	// 	polls->allowPollWrite(socket, value);
	}
	else
		Log::Error("Polls not found on Client " + SUtils::longToString(id));
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
