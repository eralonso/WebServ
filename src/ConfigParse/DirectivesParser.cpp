/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:41:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/15 19:36:19 by eralonso         ###   ########.fr       */
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
					StringVector allowedDirectives )
{
	Directives	*d = new Directives;

	while ( content.length() > 0 )
	{
		try
		{
			parseLine( d, content, allowedDirectives );
		}
		catch ( const std::exception& e )
		{
			delete d;
			throw std::logic_error( e.what() );
		}
	}
	//d->print();
	return ( d );
}

//check if line is correct
void	DirectivesParser::parseLine( Directives *d, std::string& content, \
					StringVector allowedDirectives )
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
		checkDuplicateDirective( name, d->_isSet );
		d->_isSet[ name ] = true;
		d->_isEmpty = false;
		parseDirective( head, body, d );
	}
	else if ( content.length() > 0 )
		throw std::logic_error( "Unxpected \"}\" or end of file" );
}

//check directive
void	DirectivesParser::checkValidDirective( std::string directive, \
						StringVector allowedDirectives )
{
	if ( STLUtils::stringEasyFind< StringVector >( \
				directivesList.begin(), \
				directivesList.end(), directive ) < 0 )
		throw std::logic_error( UNKNOWN_DIRECTIVE( directive ) );
	else if ( STLUtils::stringEasyFind< StringVector >( \
				allowedDirectives.begin(), \
				allowedDirectives.end(), directive ) < 0 )
		throw std::logic_error( "\"" + directive \
					+ "\" directive isn't valid in this block" );
}

void	DirectivesParser::checkValidSeparator( int type, std::string directive )
{
	static std::string	separators[ 2 ] = { ";", "{" };
	int					expectedSeparator;

	if ( STLUtils::stringEasyFind< StringVector >( \
				simpleDirectivesList.begin(), \
				simpleDirectivesList.end(), directive ) >= 0 )
		expectedSeparator = SEMICOLON_SEPARATOR;
	else
		expectedSeparator = BRACET_SEPARATOR;
	if ( type != expectedSeparator )
		throw std::logic_error( "\"" + directive \
				+ "\" invalid separator for this directive, expected \"" \
				+ separators[ expectedSeparator - 1 ] + "\"" );
}

void	DirectivesParser::checkDuplicateDirective( const std::string directive, \
	 					StringBoolMap isSet )
{
	if ( isSet[ directive ] == true \
				&& canRepeatDirectiveList.at( directive ) == false )
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
	if ( ( idx = STLUtils::stringEasyFind< StringVector >( \
					simpleDirectivesList.begin(), \
					simpleDirectivesList.end(), name ) ) >= 0 )
		_parseSimple[ idx ]( body, d );
	else if ( ( idx = STLUtils::stringEasyFind< StringVector >( \
					complexDirectivesList.begin(), \
					complexDirectivesList.end(), name ) ) >= 0 )
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
	if ( d->_isSet[ "alias" ] == true )
		throw std::logic_error( "\"root\" directive is duplicate, \"" \
				+ std::string( "alias" ) + "\" directive was specified earlier" );
	d->_root = args[ 0 ];
}

//listen <host>[:<port>] / <host> / <port>
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

//upload_store <path>
void	DirectivesParser::parseUploadStore( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "upload_store" ) ) );
	d->_uploadStore = args[ 0 ];
}

//index {list of index files}
void	DirectivesParser::parseIndex( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "index" ) ) );
	d->_index.insert( d->_index.begin(), args.begin(), args.end() );
}

//autoindex on/off
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

//alias <path>
void	DirectivesParser::parseAlias( std::string body, Directives *d )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "alias" ) ) );
	if ( d->_isSet[ "root" ] == true )
		throw std::logic_error( "\"alias\" directive is duplicate, \"" \
				+ std::string( "root" ) + "\" directive was specified earlier" );
	d->_alias = args[ 0 ];
}

//return <code> [<http://uri>/<https://uri>] / <code> / <http://uri> / <https://uri>
void	DirectivesParser::parseReturn( std::string body, Directives *d )
{
	StringVector	args;
	int				code;
	std::string		uri;

	code = HTTP_MOVED_TEMPORARILY;
	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 || args.size() > 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "return" ) ) );
	if ( args.size() == 1 )
	{
		if ( isHttpPrefix( args[ 0 ] ) == true )
			uri = args[ 0 ];
		else
			code = parseErrorCode( args[ 0 ], 0, 999 );
	}
	else
	{
		code = parseErrorCode( args[ 0 ], 0, 999 );
		uri = args[ 1 ];
	}
	if ( code < 0 )
		throw std::logic_error( "Invalid return code \"" \
				+ args[ 0 ] + "\" in \"return\" directive" );
	d->_return = ReturnPair( code, uri );
}

bool	DirectivesParser::isHttpPrefix( std::string uri )
{
	std::string	http( HTTP_PREFIX );
	std::string	https( HTTPS_PREFIX );

	if ( uri.compare( 0, http.length(), http ) == 0 )
		return ( true );
	else if ( uri.compare( 0, https.length(), https ) == 0 )
		return ( true );
	return ( false );
}

//allow_methods {list of allowed methods in this block}
void	DirectivesParser::parseAllowMethods( std::string body, Directives *d )
{
	StringVector	args;
	int				method;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "allow_methods" ) ) );
	for ( StringVector::iterator it = args.begin(); it != args.end(); it++ )
	{
		if ( ( method = isHttpMethod( *it ) ) == 0 )
			throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
						std::string( "allow_methods" ), *it ) );
		else
			d->_allowMethods.setAction( method );
	}
}

int	DirectivesParser::isHttpMethod( std::string method )
{
	std::string	methods[ 3 ] = { "GET", "POST", "DELETE" };
	int			code;

	for ( code = 0; code < 3; code++ )
		if ( method == methods[ code ] )
			break ;
	if ( code == 3 )
		return ( 0 );
	return ( 1 << code );
}

//cgi <cgi_extension> <cgi_interpreter>
void	DirectivesParser::parseCgi( std::string body, Directives *d )
{
	StringVector	args;
	CGIService		cgi;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "cgi" ) ) );
	if ( args[ 0 ].find( "." ) == std::string::npos )
		throw std::logic_error( "\"cgi\" directive, invalid cgi extension" );
	cgi.setExtension( args[ 0 ] );
	cgi.setInterpreter( args[ 1 ] );
	d->_cgi[ cgi.getExtension() ] = cgi;
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
	//Log::Info( "[ Config ] SERVER\n" );
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
	if ( lc._path[ lc._path.length() - 1 ] == '/' )
		lc._isDir = true;
	SUtils::split( lc._splitedPath, lc._path, "/" );
	//Log::Info( "[ Config ] LOCATION " + lc._path + "\n" );
	lc._directives = DirectivesParser::parseDirectives( body, \
						Location::allowedDirectives );
	d->_locations.insert( lc );
}
