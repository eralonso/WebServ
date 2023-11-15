/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:36:45 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 13:21:17 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>

ServerConfig::ServerConfig( void ): _clientMaxBodySize( 0 ) {}

ServerConfig::ServerConfig( const ServerConfig& s ): _ports( s._ports ), \
								_address( s._address ), \
								_locations( s._locations ), \
								_rootDir( s._rootDir ), \
								_serverName( s._serverName ), \
								_clientMaxBodySize( s._clientMaxBodySize ), \
								_errorPages( s._errorPages ) {}

ServerConfig::~ServerConfig( void ) {}

ServerConfig&	ServerConfig::operator=( const ServerConfig& s )
{
	if ( this != &s )
	{
		this->_ports = s._ports;
		this->_address = s._address;
		this->_locations = s._locations;
		this->_rootDir = s._rootDir;
		this->_serverName = s._serverName;
		this->_clientMaxBodySize = s._clientMaxBodySize;
		this->_errorPages = s._errorPages;
	}
	return ( *this );
}

PortsVector&	ServerConfig::getPorts( void ) const
{
	return ( this->_ports );
}

std::string&	ServerConfig::getHost( void ) const
{
	return ( this->_address );
}

LocationsVector&	ServerConfig::getLocations( void ) const
{
	return ( this->_locations );
}

std::string&	ServerConfig::getRoot( void ) const
{
	return ( this->_rootDir );
}

std::string&	ServerConfig::getServerName( void ) const
{
	return ( this->_serverName );
}

unsigned int	ServerConfig::getClientMaxBodySize( void ) const
{
	return ( this->_clientMaxBodySize );
}

ErrorPagesMap&	ServerConfig::getErrorPagesMap( void ) const
{
	return ( this->_errorPages );
}
