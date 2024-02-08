/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientGetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:54:43 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:01:09 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool	Client::isResponsePendingToSend( void ) const
{
	return (this->res != NULL);
}

bool	Client::isResponseSent( void ) const
{
	return ( this->responseSent );
}

socket_t	Client::getClientSocket( void ) const
{
	return ( this->socket );
}

size_t	Client::getId( void ) const
{
	return ( this->id );
}

const struct sockaddr_in&	Client::getAddr( void ) const
{
	return ( this->addr );
}

std::string	Client::getIpString( void ) const
{
	return ( Binary::decodeAddress( ntohl( addr.sin_addr.s_addr ) ) );
}

unsigned int	Client::getIpNetworkOrder( void ) const
{
	return ( addr.sin_addr.s_addr );
}

unsigned int	Client::getIpHostOrder( void ) const
{
	return ( ntohl( addr.sin_addr.s_addr ) );
}

const ServersVector&	Client::getServers( void ) const
{
	return ( *this->servers );
}

const std::string&	Client::getCgiOutput( void ) const
{
	return ( this->cgiOutput );
}

bool	Client::getKeepAlive( void ) const
{
	return ( this->keepAlive );
}

size_t	Client::getPendingSize( void ) const
{
	return ( this->received.size() - this->pending );
}

int	Client::getFileFd( void ) const
{
	return ( this->fileFd );
}

int	Client::getPipeCgiWrite( void ) const
{
	return ( this->pipeCgiWrite );
}

int	Client::getPipeCgiRead( void ) const
{
	return ( this->pipeCgiRead );
}

socket_t	Client::getSocket( void ) const
{
	return ( this->socket );
}

