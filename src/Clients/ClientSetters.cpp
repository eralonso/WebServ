/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:58:18 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:03:33 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool	Client::setKeepAlive( bool value )
{
	this->keepAlive = value;
	
	return ( this->keepAlive );
}

void	Client::setFileFd( int fd )
{
	this->fileFd = fd;
}

void	Client::setPipeCgiWrite( int fd )
{
	this->pipeCgiWrite = fd;
}

void	Client::setPipeCgiRead( int fd )
{
	this->pipeCgiRead = fd;
}
