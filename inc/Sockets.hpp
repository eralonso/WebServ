/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 13:56:30 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/28 11:43:51 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKETS_HPP__
# define __SOCKETS_HPP__

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

# include "Defines.hpp"
# include "Utils.hpp"

class Sockets
{
	private:
		Sockets( void );
		Sockets( const Sockets& socket );
		~Sockets( void );
		Sockets	operator=( const Sockets& socket );
	public:
		static socket_t				socketCreate( int domain, int type, int protocol );
		static struct sockaddr_in	fillSockAddr( int family, uint16_t port, uint32_t ip_addr );
		static void					bindSocket( socket_t fd, struct sockaddr_in addr );
		static void					listenFromSocket( socket_t fd, int backlog );
		static socket_t				acceptConnection( socket_t fd );
		static socket_t				createPassiveSocket( int port, int backlog );
};

#endif
