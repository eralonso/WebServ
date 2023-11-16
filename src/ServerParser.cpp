/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 19:40:26 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

ServerParser::ServerParser( std::string options ): ServerConfig()
{
	std::string content;
	std::string head;
	std::string body;
	std::array< std::string, SIZE_SERVER_OPTIONS - 1 >::iterator	it;
	std::array< std::string, SIZE_SERVER_OPTIONS - 1 >	availablesOptions = { \
										"root", "listen", \
										"server_name", "error_page", \
										"client_max_body_size" };
	std::array< t_parse, SIZE_SERVER_OPTIONS - 1 >	parseOption = { \
					&ServerParser::parseRoot, &ServerParser::parseListen, \
					&ServerParser::parseServerName, &ServerParser::parseErrorPage, \
					&ServerParser::parseClientMaxBodySize };

	content = options;
	while ( content.length() > 0 )
	{
		//std::cout << "ServerParser -> content <Before> [" << content << "] <-" << std::endl;
		if ( TreeSplit::get_pair( head, body, content ) )
		{
			it = std::find( availablesOptions.begin(), availablesOptions.end(), head );
			if ( it == availablesOptions.end() )
			{
			//	std::cout << "ServerParser -> content <After> [" << content << "]" << std::endl;
			//	std::cout << "ServerParser -> head[" << head << "]" << std::endl;
			//	std::cout << "ServerParser -> body[" << body << "]\n\n" << std::endl;
				if ( head.compare( 0, 8, "location" ) == 0 && \
						( head.length() == 8 || std::isspace( head[ 8 ] ) ) )
					parseLocation( head, body );
				else
					throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
			}
			else
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
	this->_rootDir = args[ 0 ];
}

void	ServerParser::parseLocation( std::string head, std::string body )
{
	( void )body;
	( void )head;
}

void	ServerParser::parseListen( std::string body )
{
	( void )body;
}

void	ServerParser::parseServerName( std::string body )
{
	if ( body == "" )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( std::string( "server_name" ) ) );
	this->_serverName = body;
}

void	ServerParser::parseErrorPage( std::string body )
{
	( void )body;
}

void	ServerParser::parseClientMaxBodySize( std::string body )
{
	std::vector< std::string >	args;
	size_t						init;
	std::string					num;
	std::string					maxValue;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	if ( SUtils::isNum( args[ 0 ] ) == false )
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	num = args[ 0 ];
	init = num.find_first_not_of( "0" );
	if ( init == std::string::npos )
		num = "0";
	else
		num.erase( 0, init + 1 );
	maxValue = SUtils::longToString( std::numeric_limits< long >::max() );
	if ( SUtils::compareNumbersAsStrings( num, maxValue ) > 0 )
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( std::string( "client_max_body_size" ) ) );
	this->_clientMaxBodySize = std::atol( args[ 0 ].c_str() );
}
