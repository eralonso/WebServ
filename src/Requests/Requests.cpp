/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:58:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/28 11:19:35 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Requests.hpp"

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
