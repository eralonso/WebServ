/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/01 13:53:16 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigParser.hpp>

ConfigParser::ConfigParser( int argc, char **argv ): _directives( NULL )
{
	StringVector	allowedComplexDirectives;
	StringVector	allowedSimpleDirectives;

	allowedComplexDirectives.push_back( "server" );
	checkUsage( argc, argv, argv[ 0 ] );
	readConfig();
	this->_directives = DirectivesParser::parseDirectives( this->_content, \
							allowedSimpleDirectives, allowedComplexDirectives );
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

//void	ConfigParser::parseConfigFile( void )
//{
//	std::string	head;
//	std::string	body;
//
//	while ( this->_content.length() > 0 )
//	{
//		if ( TreeSplit::get_pair( head, body, this->_content ) )
//		{
//			if ( head != "server" )
//				throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
//			ServerParser	sp( body );
//			this->_servers.push_back( Server( sp ) );
//		}
//		else if ( this->_content.length() > 0 )
//			throw std::logic_error( "Unexpected end of file, expecting \";\" or \"}\"" );
//		head.clear();
//		body.clear();
//	}
//}

ServersVector	ConfigParser::getServers( void ) const
{
	return ( this->_directives->_servers );
}
