/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/06 10:56:33 by omoreno-         ###   ########.fr       */
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

Clients::Clients(const Clients&): std::map<socket_t, Client*>()
{
}

Clients& Clients::operator=(const Clients&)
{
	return (*this);
}

Client* Clients::newClient(socket_t socket, WSPoll& polls)
{
	Client* cli = new Client(socket, polls);
	if (!cli)
		return (NULL);
	insert(std::pair<socket_t, Client*>(socket, cli));
	cli->cgis.appendCgi("py", "/usr/bin/python");
	return (cli);
}

size_t Clients::eraseClient(Client* cli)
{
	if (cli)
	{
		socket_t socket = cli->getClientSocket();
		size_t s = erase(socket);
		delete cli;
		return (s);
	}
	return (0);
}

size_t Clients::eraseClient(socket_t socket)
{
	return (erase(socket));
}

bool Clients::checkPendingToSend()
{
	Clients::iterator it = begin();
	Clients::iterator ite = end();
	// if (size()==0)
	// 	Log::Info("No Client to check");
	bool somePending = false;
	while (it != ite)
	{
		somePending |= it->second->checkPendingToSend();
		it++;
	}
	return somePending;
}
