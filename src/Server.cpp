/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 17:47:50 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ): ServerConfig() {}

Server::Server( const Server& s ): ServerConfig( s ) {}

Server::Server( const ServerParser& sp ): ServerConfig( sp ) {}

Server::~Server( void ) {}

Server&	Server::operator=( const Server& s )
{
	ServerConfig::operator=( s );
	return ( *this );
}

Server&	Server::operator=( const ServerParser& sp )
{
	ServerConfig::operator=( sp );
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
