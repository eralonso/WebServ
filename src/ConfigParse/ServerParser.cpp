/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/29 19:44:54 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

const std::string	ServerParser::_serverAllowedDirectives[ SERVER_SIZE_DIRECTIVES ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size",
	"location"
};

const std::string	ServerParser::_serverAllowedSimpleDirectives[ \
		  							SERVER_SIZE_SIMPLE_DIRECTIVES + 1 ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size"
};

const std::string	ServerParser::_serverAllowedComplexDirectives[ \
		  							SERVER_SIZE_COMPLEX_DIRECTIVES + 1 ] = 
{
	"location"
};

ServerParser::t_parseSimpleDirectiveArray	ServerParser::_serverParseSimple = \
{
	&Directives::parseRoot,
	&Directives::parseListen,
	&Directives::parseServerNames,
	&Directives::parseErrorPage,
	&Directives::parseClientMaxBodySize
};

ServerParser::t_parseComplexDirectiveArray	ServerParser::_serverParseComplex = \
{
	&Directives::parseLocation
};

std::pair< std::string, bool >	ServerParser::_canRepeatDirectivePair[ SERVER_SIZE_DIRECTIVES + 1 ] =
{
	std::make_pair< std::string, bool > ( "root", false ),
	std::make_pair< std::string, bool > ( "listen", false ),
	std::make_pair< std::string, bool > ( "server_name", true ),
	std::make_pair< std::string, bool > ( "error_page", true ),
	std::make_pair< std::string, bool > ( "client_max_body_size", false ),
	std::make_pair< std::string, bool > ( "location", true )
};

std::map< std::string, bool >	ServerParser::_canRepeatDirective( _canRepeatDirectivePair, _canRepeatDirectivePair + SERVER_SIZE_DIRECTIVES );

ServerParser::ServerParser( std::string options ): ServerConfig()
{
	std::string						content;
	std::string						head;
	std::string						body;

	Log::Info( "Starting server configuration" );
	for ( int i = 0; i < SERVER_SIZE_DIRECTIVES; i++ )
		_isSet[ _serverAllowedDirectives[ i ] ] = false;
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
	Log::Info( "[ Config ] root -> " + this->_rootDir );
	Log::Info( "[ Config ] host -> " + this->_host );
	Log::Info( "[ Config ] port -> " + SUtils::longToString( this->_port ) );
	Log::Info( "[ Config ] location -> " );
	Log::Info( "[ Config ] server_name -> " \
			+ STLUtils::vectorToString< StringVector >( \
				this->_serverNames.begin(), this->_serverNames.end() ) );
	Log::Info( "[ Config ] error_page -> " \
			+ STLUtils::mapToString< ErrorPagesMap >( \
				this->_errorPages.begin(), this->_errorPages.end() ) );
	Log::Info( "[ Config ] client_max_body_size -> " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	std::cout << std::endl;
}

ServerParser::ServerParser( const ServerParser& s ): ServerConfig( s ) {}

ServerParser::~ServerParser( void ) {}

ServerParser&	ServerParser::operator=( const ServerParser& s )
{
	ServerConfig::operator=( s );
	return ( *this );
}

void	ServerParser::parseDirective( std::string head, std::string body )
{
	int			idx;
	std::string	name;

	if ( ( idx = Directives::isSimpleDirective( head, _serverAllowedSimpleDirectives, \
			_serverAllowedSimpleDirectives + SERVER_SIZE_SIMPLE_DIRECTIVES ) ) >= 0 )
	{
		if ( _isSet[ head ] == true && _canRepeatDirective[ head ] == false )
			throw std::logic_error( "\"" + head + "\" directive is duplicate" );
		_isSet[ _serverAllowedSimpleDirectives[ idx ] ] = true;
		_serverParseSimple[ idx ]( body );
	}
	else if ( ( idx = Directives::isComplexDirective( head, _serverAllowedComplexDirectives, \
			_serverAllowedComplexDirectives + SERVER_SIZE_COMPLEX_DIRECTIVES ) ) >= 0 )
	{
		name = head.substr( 0, head.find( ISSPACE ) );
		if ( _isSet[ name ] == true && _canRepeatDirective[ name ] == false )
			throw std::logic_error( "\"" + name + "\" directive is duplicate" );
		_isSet[ _serverAllowedComplexDirectives[ idx ] ] = true;
		( _serverParseComplex[ idx ])( head, body );
	}
	else
		throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
}
