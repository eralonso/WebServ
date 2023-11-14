/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 19:39:30 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>

ServerConfig::ServerConfig( void )
{
	clientMaxBodySize = 0;
}

ServerConfig::ServerConfig( std::string server, std::string options )
{
	std::string content;
	std::string head;
	std::string body;
	std::array< std::string, SIZE_SERVER_OPTIONS >::iterator	it;
	std::array< std::string, SIZE_SERVER_OPTIONS >	availablesOptions = { \
										"root", "location", \
										"listen", "server_name", \
										"error_page", "client_max_body_size" };
	std::array< t_parse, SIZE_SERVER_OPTIONS >	parseOption = { \
					&ServerConfig::parseRoot, &ServerConfig::parseLocation, \
					&ServerConfig::parseListen, &ServerConfig::parseServerName, \
					&ServerConfig::parseErrorPage, &ServerConfig::parseClientMaxBodySize };

	content = options;
	if ( SUtils::trim( server ) != "server" )
		throw std::logic_error( "Unkown directive \"" + server + "\"" );
	while ( content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, content ) )
		{
			it = std::find( availablesOptions.begin(), availablesOptions.end(), head );
			if ( it == availablesOptions.end() )
				throw std::logic_error( "Unknown directive \"" + head + "\"" );
			( this->*parseOption[ it - availablesOptions.begin() ] )( body );
		}
		else if ( content.length() > 0 )
			throw std::logic_error( "Unxpected \"}\"" );
		head.clear();
		body.clear();
	}
}

ServerConfig::~ServerConfig( void )
{
}

ServerConfig::ServerConfig( const ServerConfig& b )
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
}

ServerConfig&	ServerConfig::operator=( const ServerConfig& b )
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
	return ( *this );
}

void	ServerConfig::parseRoot( std::string body )
{
	std::vector< std::string >	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "root" ) ) );
	root = args[ 0 ];
}

void	ServerConfig::parseLocation( std::string body )
{
	( void )body;
}

void	ServerConfig::parseListen( std::string body )
{
	( void )body;
}

void	ServerConfig::parseServerName( std::string body )
{
	if ( body == "" )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "server_name" ) ) );
	server_name = body;
}

void	ServerConfig::parseErrorPage( std::string body )
{
	( void )body;
}

void	ServerConfig::parseClientMaxBodySize( std::string body )
{
	std::vector< std::string >	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "client_max_body_size" ) ) );
	if ( isNum( args[ 0 ] ) == false && args[ 0 ] > std::numeric_limits< long >::max() )
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( std::string( "client_max_body_size" ) ) );
	clientMaxBodySize = std::atol( args[ 0 ] );
}
