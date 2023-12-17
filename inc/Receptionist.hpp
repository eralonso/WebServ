/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:43:42 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 12:18:15 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RECEPTIONIST_HPP_
# define _RECEPTIONIST_HPP_

# include <iostream>

# include <unistd.h>

# include <WSSignals.hpp>
# include <Defines.hpp>
# include <TypesDefines.hpp>
# include "Sockets.hpp"
# include "WSPoll.hpp"
# include "Utils.hpp"
# include "Clients.hpp"
# include "CgiExecutor.hpp"
# include <Directives.hpp>

class Receptionist: public Clients
{
private:
	WSPoll			polls;
	ServersVector	_servers;
	int				timeout;
public:
	Receptionist( ServersVector servers );
	~Receptionist( void );
	Receptionist( const Receptionist& b );
	Receptionist& 	operator=( const Receptionist& b );
	int				mainLoop( void );
	static int		sendResponse( socket_t connected, std::string response );
	static int		readRequest( socket_t clientFd, std::string& readed );
	int				addNewClient( socket_t serverFd );
	void			manageClient( socket_t clientFd );
	void			manageClientRead( socket_t clientFd, Client *cli );
	void			manageClientWrite( socket_t clientFd, Client *cli );
};

#endif
