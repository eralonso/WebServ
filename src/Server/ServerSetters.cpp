/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/13 12:28:18 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Receptionist.hpp>

void	Server::setAddr( const struct sockaddr_in& info )
{
	this->addr = info;
}

void	Server::setSocketFd( socket_t fd )
{
	this->_socketFd = fd;
}

void	Server::setReceptionist( Receptionist *recp )
{
	this->receptionist = recp;
}

void	Server::setEvents( Events *bEvs )
{
	this->evs = bEvs;
}

int	Server::setEventRead( void )
{
	if ( this->evs )
		return ( this->evs->setEventRead( this, this->_socketFd ) );
	return ( 0 );
}
