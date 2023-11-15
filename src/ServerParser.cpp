/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 14:26:27 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

ServerParser::ServerParser( std::string server, std::string options ): ServerConfig()
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
					&ServerParser::parseRoot, &ServerParser::parseLocation, \
					&ServerParser::parseListen, &ServerParser::parseServerName, \
					&ServerParser::parseErrorPage, &ServerParser::parseClientMaxBodySize };

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

ServerParser::~ServerParser( void ) {}

ServerParser::ServerParser( const ServerParser& s ): ServerConfig( s ) {}

ServerParser&	ServerParser::operator=( const ServerParser& s )
{
	ServerConfig::operator=( s );
	return ( *this );
}

void	ServerParser::parseRoot( std::string body )
{
	std::vector< std::string >	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "root" ) ) );
	root = args[ 0 ];
}

void	ServerParser::parseLocation( std::string body )
{
	( void )body;
}

void	ServerParser::parseListen( std::string body )
{
	( void )body;
}

void	ServerParser::parseServerName( std::string body )
{
	if ( body == "" )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "server_name" ) ) );
	server_name = body;
}

void	ServerParser::parseErrorPage( std::string body )
{
	( void )body;
}

void	ServerParser::parseClientMaxBodySize( std::string body )
{
	std::vector< std::string >	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	if ( isNum( args[ 0 ] ) == false || \
			SUtils::compareNumbersAsStrings( \
				args[ 0 ], \
				SUtils::longToString( std::numeric_limits< long >::max() ) > 0 ) )
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( std::string( "client_max_body_size" ) ) );
	clientMaxBodySize = std::atol( args[ 0 ] );
}
