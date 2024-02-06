/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:36 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/06 18:30:24 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENTS_HPP_
# define _CLIENTS_HPP_

# include <Client.hpp>
# include <map>
# include <Sockets.hpp>

class Clients: public std::map< socket_t, Client * >
{
private:
public:
	Clients( void );
	virtual ~Clients( void );
	Clients( const Clients& b );
	Clients&	operator=( const Clients& b );
	Client		*newClient( socket_t socket, Events *bEvs, \
							const ServersVector *servers, struct sockaddr_in& info, \
	   						Receptionist *recp );
	size_t		eraseClient( Client* cli );
	size_t		eraseClient( socket_t socket );
	bool		checkPendingToSend( void );
};

#endif
