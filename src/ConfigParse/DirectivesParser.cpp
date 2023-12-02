/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:41:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 19:18:03 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <DirectivesParser.hpp>

DirectivesParser::DirectivesParser( void ) {}

DirectivesParser::DirectivesParser( const DirectivesParser& dp )
{
	( void )dp;
}

DirectivesParser::~DirectivesParser( void ) {}

DirectivesParser&	DirectivesParser::operator=( const DirectivesParser& dp )
{
	( void )dp;
	return ( *this );
}

Directives	*DirectivesParser::parseDirectives( std::string content, \
					ConstStringVector allowedDirectives )
{
	ConstStringBoolMap	isSet;
	Directives			*d = new Directives;

	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		isSet[ _directivesList[ i ] ] = false;
	while ( content.length() > 0 )
	{
		try
		{
			parseLine( isSet, d, content, allowedDirectives );
		}
		catch ( const std::exception& e )
		{
			delete d;
			throw std::logic_error( e.what() );
		}
	}
	return ( d );
}

//check if line is correct
void	DirectivesParser::parseLine( ConstStringBoolMap& isSet, \
					Directives *d, std::string& content, \
					ConstStringVector allowedDirectives )
{
	std::string			head;
	std::string			body;
	int					type;
	std::string			name;

	if ( ( type = TreeSplit::get_pair( head, body, content ) ) != NOT_A_SEPARATOR )
	{
		name = head.substr( 0, head.find_first_of( ISSPACE ) );
		checkValidDirective( name, allowedDirectives );
		checkValidSeparator( type, name );
		checkDuplicateDirective( name, isSet );
		isSet[ name ] = true;
		parseDirective( head, body, d );
	}
	else if ( content.length() > 0 )
		throw std::logic_error( "Unxpected \"}\" or end of file" );
}

//check directive
void	DirectivesParser::checkValidDirective( std::string directive, \
						ConstStringVector allowedDirectives )
{
	if ( STLUtils::stringEasyFind< ConstStringVector >( \
				_directivesList.begin(), \
				_directivesList.end(), directive ) < 0 )
		throw std::logic_error( UNKNOWN_DIRECTIVE( directive ) );
	else if ( STLUtils::stringEasyFind< ConstStringVector >( \
				allowedDirectives.begin(), \
				allowedDirectives.end(), directive ) < 0 )
		throw std::logic_error( "\"" + directive \
					+ "\" directive isn't valid in this block" );
}

void	DirectivesParser::checkValidSeparator( int type, std::string directive )
{
	static std::string	separators[ 2 ] = { ";", "{" };
	int					expectedSeparator;

	if ( STLUtils::stringEasyFind< ConstStringVector >( \
				_simpleDirectivesList.begin(), \
				_simpleDirectivesList.end(), directive ) >= 0 )
		expectedSeparator = SEMICOLON_SEPARATOR;
	else
		expectedSeparator = BRACET_SEPARATOR;
	if ( type != expectedSeparator )
		throw std::logic_error( "\"" + directive \
				+ "\" invalid separator for this directive, expected \"" \
				+ separators[ expectedSeparator - 1 ] + "\"" );
}

void	DirectivesParser::checkDuplicateDirective( const std::string directive, \
	 					ConstStringBoolMap isSet )
{
	if ( isSet[ directive ] == true \
				&& _canRepeatDirectiveList.at( directive ) == false )
		throw std::logic_error( "\"" + directive \
					+ "\" directive is duplicate" );
}

//select parse function
void	DirectivesParser::parseDirective( std::string head, std::string body, \
	   						Directives *d )
{
	int			idx;
	std::string	name;

	name = head.substr( 0, head.find_first_of( ISSPACE ) );
	if ( ( idx = STLUtils::stringEasyFind< ConstStringVector >( \
					_simpleDirectivesList.begin(), \
					_simpleDirectivesList.end(), name ) ) >= 0 )
		_parseSimple[ idx ]( body, d );
	else if ( ( idx = STLUtils::stringEasyFind< ConstStringVector >( \
					_complexDirectivesList.begin(), \
					_complexDirectivesList.end(), name ) ) >= 0 )
		_parseComplex[ idx ]( head, body, d );
}

//root <path>
void	DirectivesParser::parseRoot( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "root" ) ) );
	d->_root = args[ 0 ];
}

//listen
void	DirectivesParser::parseListen( std::string body, Directives *d )
{
	StringVector	args;
	size_t			sep;
	std::string		host;
	std::string		port;
	int				ret;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "listen" ) ) );
	ret = 0;
	port = "8000";
	host = "0.0.0.0";
	sep = args[ 0 ].find( ":" );
	if ( sep != std::string::npos )
	{
		port = parsePort( args[ 0 ].substr( sep + 1, std::string::npos ), ret );
		if ( ret >= 0 )
			host = parseHost( args[ 0 ].substr( 0, sep ), ret );
	}
	else if ( args[ 0 ].find( "." ) == std::string::npos )
		port = parsePort( args[ 0 ], ret );
	else
		host = parseHost( args[ 0 ], ret );
	if ( ret < 0 )
		throw std::logic_error( parseListenStrError( ret, args[ 0 ] ) );
	if ( checkAvailableHostPort( host, port ) )
		throw std::logic_error( "host not found \"" + host + "\"" );
	d->_host = host;
	d->_port = std::atoi( port.c_str() );
}

//server_name {list of server names}
void	DirectivesParser::parseServerNames( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server_name" ) ) );
	d->_serverNames.insert( d->_serverNames.end(), args.begin(), args.end() );
}

//error_page [ code - uri ]
//code: value in range 300 - 599
//uri: internal redirect to 'uri' with client request method changed to 'GET'
void	DirectivesParser::parseErrorPage( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() < 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "error_page" ) ) );
	fillErrorPages( args, d );
}

//client_max_body_size <size>
void	DirectivesParser::parseClientMaxBodySize( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	try
	{
		d->_clientMaxBodySize = parseSize( args[ 0 ] );
	}
	catch ( const std::logic_error& e )
	{
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "client_max_body_size" ), args[ 0 ] ) );
	}
}

//index
void	DirectivesParser::parseIndex( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "index" ) ) );
	d->_index.insert( d->_index.begin(), args.begin(), args.end() );
}

//autoindex
void	DirectivesParser::parseAutoindex( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "index" ) ) );
	if ( args[ 0 ] == "on" )
		d->_autoindex = true;
	else if ( args[ 0 ] == "off" )
		d->_autoindex = false;
	else
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "autoindex" ), args[ 0 ] ) );
}

//alias
void	DirectivesParser::parseAlias( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "root" ) ) );
	d->_alias = args[ 0 ];
}

//rewrite
void	DirectivesParser::parseReturn( std::string body, Directives *d )
{
	( void )body;
	( void )d;
}

//allow_methods
void	DirectivesParser::parseAllowMethods( std::string body, Directives *d )
{
	( void )body;
	( void )d;
}

//cgi
void	DirectivesParser::parseCgi( std::string body, Directives *d )
{
	( void )body;
	( void )d;
}

//server
void	DirectivesParser::parseServer( std::string head, std::string body, \
						Directives *d )
{
	StringVector	args;
	Server			s;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() > 1 )
	{
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server" ) ) );
	}
	s._directives = DirectivesParser::parseDirectives( body, \
						Server::allowedDirectives );
	d->_servers.push_back( s );
}

//location <path>
void	DirectivesParser::parseLocation( std::string head, std::string body, \
						Directives *d )
{
	StringVector	args;
	Location		lc;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() != 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "location" ) ) );
	args.erase( args.begin() );
	lc._path = args.front();
	if ( lc._path.back() == '/' )
		lc._isDir = true;
	SUtils::split( lc._splitedPath, lc._path, "/" );
	lc._directives = DirectivesParser::parseDirectives( body, \
						Location::allowedDirectives );
	d->_locations.insert( lc );
}
