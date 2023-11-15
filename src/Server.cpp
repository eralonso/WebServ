/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 13:16:59 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ): ServerConfig() {}

Server::Server( const Server& s ): ServerConfig( s ) {}

Server::~Server( void ) {}

Server&	Server::operator=( const Server& s )
{
	ServerConfig::operator=( s );
	return ( *this );
}

Location&		getLocationAtPath( std::string path ) const
{
	( void ) path;
	return ( _locations[ 0 ] );
}

std::string&	getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}
