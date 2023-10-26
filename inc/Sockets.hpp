/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:31:17 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/26 17:44:21 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKETS_H__
# define __SOCKETS_H__

#include <sys/socket.h>
#include <netinet/in.h>

typedef int	socket_t;

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
