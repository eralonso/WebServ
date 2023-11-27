/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:41:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 13:47:53 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Clients.hpp"

Clients::Clients()
{
}

Clients::~Clients()
{
}

Client* Clients::newClient(struct pollfd* poll)
{
	if (!poll)
		return (nullptr);
	Client* cli = new Client(poll);
	if (!cli)
		return (nullptr);
	insert(std::pair<struct pollfd*, Client*>(poll, cli));
	return (cli);
}

int Clients::eraseClient(Client* cli)
{
	if (cli)
	{
		delete cli;
		struct pollfd* poll = cli->getClientPoll();
		if (poll)
			return (erase(poll));
	}
	return (0);
}

int Clients::eraseClient(struct pollfd* poll)
{
	if (poll)
		return (erase(poll));
	return (0);
}
