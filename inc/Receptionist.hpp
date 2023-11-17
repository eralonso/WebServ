/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:43:42 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 13:29:16 by omoreno-         ###   ########.fr       */
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
#include "Requests.hpp"

class Receptionist
{
private:
	WSPoll			polls;
	Requests		requests;
	int				port;
	int				backlog;
	int				timeout;
public:
	Receptionist(int port = 9375, int backlog = 20, int timeout = -1);
	~Receptionist();
	Receptionist(const Receptionist& b);
	Receptionist& 	operator=(const Receptionist& b);
	int				addNewClient( socket_t serverFd, WSPoll& polls );
	int				mainLoop(void);
	std::string		getHtml( void );
	std::string		getHeader( void );
	std::string		getResponse( void );
	std::string		getResponse( Request *req );
	void			sendResponse( socket_t connected, std::string response );
	int				readRequest( socket_t clientFd, std::string& readed );
	void			manageClient( socket_t clientFd, WSPoll& polls );
};

#endif
