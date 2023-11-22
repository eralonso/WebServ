/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 19:39:42 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigParser.hpp>

ConfigParser::ConfigParser( int argc, char **argv )
{
	checkUsage( argc, argv, argv[ 0 ] );
	readConfig();
	parseConfigFile();
}

ConfigParser::~ConfigParser( void ) {}

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

void	ConfigParser::parseConfigFile( void )
{
	std::string	head;
	std::string	body;

	while ( this->_content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, this->_content ) )
		{
			//std::cout << "ConfigParser -> this->_content [" << this->_content << "] <-" << std::endl;
			//std::cout << "ConfigParser -> head [" << head << "] <-" << std::endl;
			//std::cout << "ConfigParser -> body [" << body << "] <-" << std::endl;
			if ( head != "server" )
				throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
			ServerParser	sp( body );
			this->_servers.push_back( Server( sp ) );
		}
		else if ( this->_content.length() > 0 )
			throw std::logic_error( "Unexpected end of file, expecting \";\" or \"}\"" );
		head.clear();
		body.clear();
	}
}

std::vector<Server>	ConfigParser::getServers( void ) const
{
	return ( this->_servers );
}
