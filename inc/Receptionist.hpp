/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:43:42 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 11:10:11 by omoreno-         ###   ########.fr       */
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
# include "Utils.hpp"
# include "Clients.hpp"
# include "CgiExecutor.hpp"
# include <Directives.hpp>
# include <EventsTarget.hpp>
# include "Log.hpp"

# define BACKLOG 511

class Receptionist: public Clients, public EventsTarget
{
private:
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
	int				onEvent( Event &tevent );
	const ServersVector&	getServers( void ) const;
};

#endif
