/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 19:45:22 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

ServerParser::ServerParser( std::string options ): ServerConfig()
{
	std::string content;
	std::string head;
	std::string body;

	content = options;
	while ( content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, content ) )
			parseDirective( head, body );
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

void	ServerParser::parseDirective( std::string head, std::string body )
{
	int								idx;
	t_parseSimpleDirectiveArray		parseSimple = { \
					&ServerParser::parseRoot, &ServerParser::parseListen, \
					&ServerParser::parseServerName, &ServerParser::parseErrorPage, \
					&ServerParser::parseClientMaxBodySize };
	t_parseComplexDirectiveArray	parseComplex = { \
					&ServerParser::parseLocation };

	if ( ( idx = isSimpleDirective( head ) ) >= 0 )
		( this->*parseSimple[ idx ] )( body );
	else if ( ( idx = isComplexDirective( head ) ) >= 0 )
		( this->*parseComplex[ idx ] )( head, body );
	else
		throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
}

int	ServerParser::isSimpleDirective( std::string head )
{
	simpleDirectiveArray::iterator	it;
	simpleDirectiveArray			simpleDirective = { \
												"root", "listen", \
												"server_name", "error_page", \
												"client_max_body_size" };

	it = std::find( simpleDirective.begin(), simpleDirective.end(), head );
	if ( it == simpleDirective.end() )
		return ( -1 );
	return ( it - simpleDirective.begin() );
}

int	ServerParser::isComplexDirective( std::string head )
{
	std::vector< std::string >		args;
	simpleDirectiveArray::iterator	it;
	simpleDirectiveArray			complexDirective = { \
												"location" };

	SUtils::split( args, head, ISSPACE );
	it = std::find( complexDirective.begin(), complexDirective.end(), args[ 0 ] );
	if ( it == complexDirective.end() )
		return ( -1 );
	return ( it - complexDirective.begin() );
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

//TODO: Accept unit measures like m/M(Megabytes)
//i.e: client_max_body_size 1M;
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
