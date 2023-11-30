/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 19:10:28 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ): _port( 8000 ), \
								_host( "0.0.0.0" ), \
								_clientMaxBodySize( 1 << 20 ) {}

Directives::~Directives( void ) {}

Directives::Directives( const Directives& d ):
								_root( d._root ), \
								_port( d._port ), \
								_host( d._host ), \
								_serverNames( d._serverNames ), \
								_errorPages( d._errorPages ), \
								_clientMaxBodySize( d._clientMaxBodySize ), \
								_index( d._index ), \
								_autoindex( d._autoindex ), \
								_alias( d._alias ), \
								_return( d._return ), \
								_allowMethods( d._allowMethods ), \
								_cgi( d._cgi ), \
								_servers( d._servers ), \
								_locations( d._locations ) {}

Directives&	Directives::operator=( const Directives& d )
{
	if ( this != &d )
	{
		this->_root = s._root;
		this->_port = s._port;
		this->_host = s._host;
		this->_serverNames = s._serverNames;
		this->_errorPages = s._errorPages;
		this->_clientMaxBodySize = s._clientMaxBodySize;
		this->_index = s._index;
		this->_autoindex = s._autoindex;
		this->_alias = s._alias;
		this->_return = s._return;
		this->_allowMethods = s._allowMethods;
		this->_cgi = s._cgi;
		this->_servers = s._servers;
		this->_locations = s._locations;
	}
	return ( *this );
}
