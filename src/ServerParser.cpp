/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/18 15:29:59 by eralonso         ###   ########.fr       */
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
long	ServerParser::getMeasureLimit( int unit )
{
	return ( LONG_MAX >> ( 10 * unit ) );
}

int	ServerParser::parseMeasure( std::string number )
{
	std::array< char, 4 >			units = { 'b', 'k', 'm', 'g' };
	std::array< char, 4 >::iterator	it;
	size_t							i;
	char							unit;
	int								res;

	res = 0;
	i = number.find_first_not_of( "0123456789" );
	if ( i != std::string::npos )
	{
		unit = std::tolower( number[ i ] );
		it = std::find( units.begin(), units.end(), unit );
		if ( it == units.end() || number[ i + 1 ] != '\0' )
			return ( -1 );
		res = it - units.begin();
	}
	return ( res );
}

long	ServerParser::parseSize( std::string number )
{
	int		measure;

	measure = parseMeasure( number );
	if ( measure == -1 )
		throw std::logic_error( "Invalid measurement unit" );
	if ( number.find_first_not_of( "0123456789" ) != std::string::npos )
		number.erase( number.length() - 1, 1 );
	if ( SUtils::compareNumbersAsStrings( number, \
				SUtils::longToString( getMeasureLimit( measure ) ) ) > 0 )
		throw std::logic_error( "Number too large" );
	return ( std::atol( number.c_str() ) << ( 10 * measure ) );
}

void	ServerParser::parseClientMaxBodySize( std::string body )
{
	std::vector< std::string >	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	try
	{
		this->_clientMaxBodySize = parseSize( args[ 0 ] );
		Log::Info( "client_max_body_size: " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	}
	catch ( const std::logic_error& e )
	{
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	}
}
