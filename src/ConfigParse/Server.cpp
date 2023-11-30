/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 18:33:54 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ) {}

Server::Server( const Server& s ): _directives( s._directives ) {}

Server::~Server( void ) {}

Server&	Server::operator=( const Server& s )
{
	if ( this != &s )
		this->_directives = s._directives;
	return ( *this );
}

Location	Server::getLocationAtPath( std::string path ) const
{
	( void ) path;
	return ( this->_locations[ 0 ] );
}

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}
