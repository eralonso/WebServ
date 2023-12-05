/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/05 12:56:13 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Clients.hpp"
#include "Clients.hpp"

Clients::Clients()
{
}

Clients::~Clients()
{
}

Clients::Clients(const Clients&)
{
	;
}

Clients& Clients::operator=(const Clients&)
{
	return (*this);
}

Client* Clients::newClient(struct pollfd* poll)
{
	if (!poll)
		return (nullptr);
	Client* cli = new Client(poll);
	if (!cli)
		return (nullptr);
	insert(std::pair<struct pollfd*, Client*>(poll, cli));
	cli->cgis.appendCgi("py", "/usr/bin/python");
	return (cli);
}

int Clients::eraseClient(Client* cli)
{
	if (cli)
	{
		struct pollfd* poll = cli->getClientPoll();
		if (poll)
			return (erase(poll));
		delete cli;
	}
	return (0);
}

int Clients::eraseClient(struct pollfd* poll)
{
	if (poll)
		return (erase(poll));
	return (0);
}

bool Clients::checkPendingToSend()
{
	Clients::iterator it = begin();
	Clients::iterator ite = end();
	if (size()==0)
		Log::Info("No Client to check");
	bool somePending = false;
	while (it != ite)
	{
		somePending |= it->second->checkPendingToSend();
		it++;
	}
	return somePending;
}
