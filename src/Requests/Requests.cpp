/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:58:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 15:00:12 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Requests.hpp"

Requests::Requests()
{
}

Requests::~Requests()
{
	while (this->size() > 0)
	{
		Request* req = this->begin()->second;
		if (req)
			eraseRequest(req);
	}
}

Requests::Requests(const Requests& b) : std::map<struct pollfd*, Request*>(b)
{
}

Requests& Requests::operator=(const Requests& b)
{
	std::map<struct pollfd*, Request*>::operator=(b);
	return (*this);
}

Request* Requests::appendRequest(struct pollfd* client)
{
	if (!client)
		return (nullptr);
	Request *req = new Request(client);
	if (!req)
		return (nullptr);
	insert(std::pair<struct pollfd*, Request*>(client, req));
	return (req);
}

int	Requests::eraseRequest(Request* req)
{
	if (req)
	{
		delete req;
		struct pollfd* client = req->getClientPoll();
		if (client)
			return (erase(client));
	}
	return (0);
}

int	Requests::eraseRequest(struct pollfd*  clientPoll)
{
	if (clientPoll)
	{
		Request* req = operator[](clientPoll);
		if (req)
			delete req;
		return (erase(clientPoll));
	}
	return (0);
}
