/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/08 19:39:41 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigParser.hpp>

ConfigParser::ConfigParser( int argc, char **argv ): _directives( NULL )
{
	ConstStringVector	allowedDirectives;

	allowedDirectives.push_back( "server" );
	checkUsage( argc, argv, argv[ 0 ] );
	readConfig();
	this->_directives = DirectivesParser::parseDirectives( this->_content, \
							allowedDirectives );
}

ConfigParser::~ConfigParser( void )
{
	if ( this->_directives != NULL )
		delete this->_directives;
}

void	ConfigParser::checkUsage( int argc, char **argv, std::string binName )
{
	if ( argc > 2 )
		throw std::logic_error( "Usage: " + binName + " {<config_file>,<nothing>}" );
	this->_fileName = ( argc == 2 ) ? argv[ 1 ] : "default.conf";
}

void	ConfigParser::readConfig( void )
{
	std::ifstream	myfile;
	std::string		content;
	std::string		line;

	myfile.open( this->_fileName, std::ios_base::in );
	if ( !myfile.is_open() )
		throw std::logic_error( "Invalid file: " + this->_fileName );
	while ( std::getline ( myfile, line ) )
		content += line + "\n";
	myfile.close();
	this->_content = SUtils::trim( content );
}

ServersVector	ConfigParser::getServers( void ) const { return ( this->_directives->_servers ); }
