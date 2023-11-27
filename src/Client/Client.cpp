/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 14:20:11 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include "Client.hpp"
#include "Response.hpp"

Client::Client(void)
{
	this->clientPoll = nullptr;
}

Client::Client(struct pollfd *clientPoll)
{
	this->clientPoll = clientPoll;
}


Client::~Client()
{
}

struct pollfd	*Client::getClientPoll()
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
		req = *it;
		if(req && req->isCompleteRecv())
			return (req);
		it++;
	}
	return nullptr;
}

Request* Client::findReadyToSendRequest()
{
	Request* req;
	Requests::iterator	it = begin();
	Requests::iterator	ite = end();
	while (it != ite)
	{
		req = *it;
		if(req && req->isReadyToSend())
			return (req);
		it++;
	}
	return nullptr;
}

int Client::manageRecv(std::string recv)
{
	Request* req = findRecvRequest();
	if (req)
	{
		req->appendRecv(recv);
		return (1);
	}
	return 0;
}

int	Client::managePollout()
{
	Request* req = findReadyToSendRequest();
	if (req)
		return (sendResponse(getResponse(req)));
	return 0;
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
		res.setStatus(500);
		res.setBody("Error: 500");
	}
	else
	{
		res.setStatus(200);
		res.setMethod(req->getMethod());
		res.setBody(getHtml());
	}
	return res.toString();
}

int	Client::sendResponse(std::string resp)
{
	if (clientPoll)
	{
		if ( send( clientPoll->fd, resp.c_str(), resp.size(), 0 ) < 0 )
		{
			Log::Error( "Failed to send response" );
			exit( 1 );
		}
		Log::Success( "Response sended [ " + SUtils::longToString( clientPoll->fd ) + " ]" );	
		return (1);
	}
	return (0);
}
