/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:58:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/05 13:14:29 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Requests.hpp"
#include <Utils.hpp>

Requests::Requests()
{
}

Requests::~Requests()
{
	while (this->size() > 0)
		eraseRequest();
}

Requests::Requests(const Requests& b) : std::vector<Request*>(b)
{
}

Requests& Requests::operator=(const Requests& b)
{
	std::vector<Request*>::operator=(b);
	return (*this);
}

Request* Requests::appendRequest(Client *cli)
{
	Request *req = new Request(cli);
	if (!req)
		return (nullptr);
	push_back(req);
	return (req);
}

int	Requests::eraseRequest()
{
	if (size() > 0)
	{
		Requests::iterator ite = end() - 1;
		Request* req = *ite;
		if (req)
		{
			erase(ite);
			delete req;
			return (1);
		}
	}
	return (0);
}

bool	Requests::checkPendingToSend()
{
	size_t pos = size();
	if (pos > 0)
	{
		Request *r = operator[](pos - 1);
		if (r->isReadyToSend())
			return 1;
		else
			Log::Info("Last Request " + SUtils::longToString(r->getId()) + " in " + SUtils::longToString(pos) + " pos, is not ReadyToSend");
	}
	else
		Log::Info("No Requests on checkPendingToSend");
	return 0;
}
