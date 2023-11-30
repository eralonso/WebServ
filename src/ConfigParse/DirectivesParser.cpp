/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:41:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 19:29:12 by eralonso         ###   ########.fr       */
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

Directives	DirectivesParser::parseDirectives( std::string content, \
					StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives )
{
	std::string							head;
	std::string							body;
	int									type;
	std::string							name;
	Directives							d;
	std::map< const std::string, bool >	isSet;

	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		isSet[ DirectivesVector[ i ] ] = false;
	while ( content.length() > 0 )
	{
		if ( ( type = TreeSplit::get_pair( head, body, content ) ) )
		{
			name = head.substr( 0, head.find( ISSPACE ) );
			checkValidDirective( name );
			checkValidSeparator( type, name );
			checkDuplicateDirective( name, isSet );
			isSet[ name ] = true;
			parseDirective( head, body, type, d, allowedSimpleDirectives, \
					allowedComplexDirectives );
		}
		else if ( content.length() > 0 )
			throw std::logic_error( "Unxpected \"}\" or end of file" );
		head.clear();
		body.clear();
	}
	return ( d );
}

//select parse function
void	DirectivesParser::parseDirective( std::string head, std::string body, \
					Directives& d, StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives )
{
	int	idx;

	if ( ( idx = isSimpleDirective( head, allowedSimpleDirectives ) ) >= 0 )
		_ParseSimple[ idx ]( body, d );
	else if ( ( idx = isComplexDirective( head, allowedComplexDirectives ) ) >= 0 )
		_ParseComplex[ idx ]( head, body, d );
}

//check directive
void	DirectivesParser::checkValidDirective( std::string directive )
{
	if ( std::find( DirectivesVector.begin(), DirectivesVector.end(), directive ) \
			== DirectivesVector.end() )
		throw std::logic_error( UNKNOWN_DIRECTIVE( directive ) );
}

void	DirectivesParser::checkValidSeparator( int type, std::string directive )
{
	static std::string	separators[ 2 ] = { ";", "{" };

	if ( type == separators[ type - 1 ] )
		throw std::logic_error( "\"" + directive \
				+ "\" invalid separator for this directive, expected \"" \
				+ separators[ type - 1 ] + "\"" );
}

void	DirectivesParser::checkDuplicateDirective( std::string directive, \
	 					std::map< const std::string, bool > isSet )
{
	if ( isSet[ directive ] == true && _canRepeatDirective[ directive ] == false )
		throw std::logic_error( "\"" + directive + "\" directive is duplicate" );
}

//root <path>
void	DirectivesParser::parseRoot( std::string body, Directives& d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "root" ) ) );
	d._root = args[ 0 ];
}

//listen
void	DirectivesParser::parseListen( std::string body, Directives& d )
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
	d._host = host;
	d._port = port;
}

//server_name {list of server names}
void	DirectivesParser::parseServerNames( std::string body, Directives& d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server_name" ) ) );
	d._serverNames.insert( d._serverNames.end(), args.begin(), args.end() );
}

//error_page [ code - uri ]
//code: value in range 300 - 599
//uri: internal redirect to 'uri' with client request method changed to 'GET'
void	DirectivesParser::parseErrorPage( std::string body, Directives& d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() < 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "error_page" ) ) );
	fillErrorPages( args, d );
}

//client_max_body_size <size>
void	DirectivesParser::parseClientMaxBodySize( std::string body, Directives& d )
{
	StringVector	args;
	long			size;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	try
	{
		d._clientMaxBodySize = parseSize( args[ 0 ] );
	}
	catch ( const std::logic_error& e )
	{
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "client_max_body_size" ), args[ 0 ] ) );
	}
}

//index
void	DirectivesParser::parseIndex( std::string body, Directives& d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "index" ) ) );
	d._index.insert( d.index.begin(), args.begin(), args.end() );
}

//autoindex
void	DirectivesParser::parseAutoindex( std::string body, Directives& d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "index" ) ) );
	if ( args[ 0 ] == "on" )
		d._autoindex = true;
	else if ( args[ 0 ] == "off" )
		d._autoindex = false;
	else
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "autoindex" ), args[ 0 ] ) );
}

//alias
void	DirectivesParser::parseAlias( std::string body, Directives& d )
{
	( void )body;
}

//rewrite
void	DirectivesParser::parseReturn( std::string body, Directives& d )
{
	( void )body;
}

//allow_methods
void	DirectivesParser::parseAllowMethods( std::string body, Directives& d )
{
	( void )body;
}

//cgi
void	DirectivesParser::parseCgi( std::string body, Directives& d )
{
	( void )body;
}

//server
void	DirectivesParser::parseServer( std::string head, std::string body, \
						Directives& d )
{
	StringVector	args;
	Server			s;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() > 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server" ) ) );
	s._directives = DirectivesParser::parseDirectives( body, \
						Server::AllowedSimpleDirectives, \
						Server::AllowedComplexDirectives );
	d._servers.push_back( s );
}

//location <path>
void	DirectivesParser::parseLocation( std::string head, std::string body, \
						Directives& d )
{
	std::string		path;
	StringVector	args;
	Location		lc;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() != 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "location" ) ) );
	args.erase( args.begin() );
	path = args.front();
	lc._directives = DirectivesParser::parseDirectives( body, \
						Location::AllowedSimpleDirectives, \
						Location::AllowedComplexDirectives );
	d._locations.push_back( lc );
}
