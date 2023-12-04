/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directived.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/04 12:35:07 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ): _port( 8000 ), \
								_host( "0.0.0.0" ), \
								_clientMaxBodySize( 1 << 20 )
{
	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		this->_isSet[ DirectivesParser::directivesList[ i ] ] = false;
}

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
								_locations( d._locations ), \
								_isSet( d._isSet ) {}

Directives&	Directives::operator=( const Directives& d )
{
	if ( this != &d )
	{
		this->_root = d._root;
		this->_port = d._port;
		this->_host = d._host;
		this->_serverNames = d._serverNames;
		this->_errorPages = d._errorPages;
		this->_clientMaxBodySize = d._clientMaxBodySize;
		this->_index = d._index;
		this->_autoindex = d._autoindex;
		this->_alias = d._alias;
		this->_return = d._return;
		this->_allowMethods = d._allowMethods;
		this->_cgi = d._cgi;
		this->_servers = d._servers;
		this->_locations = d._locations;
		this->_isSet = d._isSet;
	}
	return ( *this );
}
