/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directived.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/04 19:02:06 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ): _port( 8000 ), \
								_host( "0.0.0.0" ), \
								_clientMaxBodySize( 1 << 20 )
{
	this->_return.first = -1;
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

void	Directives::print( void ) const
{
	Log::Info( "[ Config ] root: " + this->_root );
	Log::Info( "[ Config ] host: " + this->_host );
	Log::Info( "[ Config ] port: " + SUtils::longToString( this->_port ) );
	Log::Info( "[ Config ] server_name: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_serverNames.begin(), this->_serverNames.end() ) );
	Log::Info( "[ Config ] error_page: " \
				+ STLUtils::mapToString< ErrorPagesMap >( \
				this->_errorPages.begin(), this->_errorPages.end() ) );
	Log::Info( "[ Config ] client_max_body_size: " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	Log::Info( "[ Config ] index: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_index.begin(), this->_index.end() ) );
	Log::Info( "[ Config ] autoindex: " + std::string( \
				this->_autoindex == true ? "on" : "off" ) );
	Log::Info( "[ Config ] alias: " + this->_alias );
	Log::Info( "[ Config ] return: " \
				+ SUtils::longToString( this->_return.first ) \
				+ " -> " \
				+ this->_return.second );
	Log::Info( "[ Config ] allow_methods: " \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::GET ) == true ? "GET " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::POST ) == true ? "POST " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::DELETE ) == true ? "DELETE" : "" ) );
	Log::Info( "[ Config ] cgi: " \
				+ STLUtils::mapToString< CgiMap >( \
				this->_cgi.begin(), this->_cgi.end() ) );
	Log::Info( "[ Config ] servers: " );
	Log::Info( "[ Config ] location: " );
	std::cout << std::endl;
}
