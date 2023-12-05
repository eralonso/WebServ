/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:43:42 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/05 17:26:30 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RECEPTIONIST_HPP_
# define _RECEPTIONIST_HPP_

#include <Defines.hpp>
#include <iostream>
#include <unistd.h>
#include <WSSignals.hpp>
#include "Sockets.hpp"
#include "WSPoll.hpp"
#include "Utils.hpp"
#include "Clients.hpp"
#include "CgiExecutor.hpp"

class Receptionist : public Clients
{
private:
	WSPoll							polls;
	int								port;
	int								backlog;
	int								timeout;
public:
	Receptionist(int port = 9375, int backlog = 20, int timeout = 1000);
	~Receptionist();
	Receptionist(const Receptionist& b);
	Receptionist& 	operator=(const Receptionist& b);
	int				mainLoop(void);
	static int		sendResponse( socket_t connected, std::string response );
	static int		readRequest( socket_t clientFd, std::string& readed );
	int				addNewClient( socket_t serverFd);
	void			manageClient( socket_t clientFd);
};

#endif
