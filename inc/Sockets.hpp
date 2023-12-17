/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 13:56:30 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/17 13:10:24 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SOCKETS_HPP_
# define _SOCKETS_HPP_

# include <iostream>

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# include <Defines.hpp>
# include <Utils.hpp>

class Sockets
{
	private:
		Sockets( void );
		Sockets( const Sockets& socket );
		~Sockets( void );
		Sockets	operator=( const Sockets& socket );
	public:
		static socket_t				socketCreate( int domain, int type, \
										int protocol );
		static struct sockaddr_in	fillSockAddr( int family, uint16_t port, \
										uint32_t ip_addr );
		static void					bindSocket( socket_t fd, \
										struct sockaddr *addr, socklen_t len);
		static void					listenFromSocket( socket_t fd, int backlog );
		static socket_t				acceptConnection( socket_t fd );
		static struct addrinfo		fillAddrinfo( int family, int socktype, \
										int protocol, int flags );
		static struct sockaddr		codeHost( const char *host, int port );
		static socket_t				createPassiveSocket( std::string host, \
										int port, int backlog );
};

#endif
