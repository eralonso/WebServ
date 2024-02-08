/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:43:42 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 10:39:52 by omoreno-         ###   ########.fr       */
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
// # include "WSPoll.hpp"
# include "Utils.hpp"
# include "Clients.hpp"
# include "CgiExecutor.hpp"
# include <Directives.hpp>

# define BACKLOG 200

class Receptionist: public Clients
{
private:
	Events			evs;
	ServersVector	_servers;
	int				timeout;
public:
	Receptionist( ServersVector& servers );
	~Receptionist( void );
	Receptionist( const Receptionist& b );
	Receptionist& 	operator=( const Receptionist& b );
	void			setupServers( void );
	bool			serverShareAddr( ServersVector::iterator& begin, \
											ServersVector::iterator& curr, \
											struct sockaddr_in& info, \
		   									socket_t& serverFd );
	int				mainLoop( void );
	// static int		sendResponse( socket_t connected, Response *res );
	// static int		readRequest( socket_t clientFd, std::string& readed );
	// int				addNewClient( socket_t serverFd );
	// void			manageClient( socket_t clientFd );
	// void			manageClientRead( socket_t clientFd, Client *cli );
	// void			manageClientWrite( socket_t clientFd, Client *cli );
	const ServersVector&	getServers( void ) const;
};

#endif
