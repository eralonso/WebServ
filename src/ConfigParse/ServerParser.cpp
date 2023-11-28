/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/28 17:13:55 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

const std::string	ServerParser::_directives[ SERVER_SIZE_DIRECTIVES ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size",
	"location"
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
		_isSet[ _directives[ i ] ] = false;
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
	int								idx;
	t_parseSimpleDirectiveArray		parseSimple = { \
					&ServerParser::parseRoot, &ServerParser::parseListen, \
					&ServerParser::parseServerNames, &ServerParser::parseErrorPage, \
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
	int								idx;
	std::string						*it;
	simpleDirectiveArray			simpleDirective = { \
										"root", "listen", \
										"server_name", "error_page", \
										"client_max_body_size" };

	it = std::find( simpleDirective, simpleDirective + SIZE_SIMPLE_DIRECTIVES, head );
	if ( it == simpleDirective + SIZE_SIMPLE_DIRECTIVES )
		return ( -1 );
	idx = it - simpleDirective;
	if ( _isSet[ head ] == true \
			&& _canRepeatDirective[ head ] == false )
		throw std::logic_error( "\"" + head + "\" directive is duplicate" );
	_isSet[ simpleDirective[ idx ] ] = true;
	return ( idx );
}


int	ServerParser::isComplexDirective( std::string head )
{
	int						idx;
	std::string				name;
	std::string				args;
	std::string				*it;
	complexDirectiveArray	complexDirective = { \
								"location" };

	TreeSplit::splitOnceBySpace( head, name, args );
	it = std::find( complexDirective, complexDirective + SIZE_COMPLEX_DIRECTIVES, name );
	if ( it == complexDirective + SIZE_COMPLEX_DIRECTIVES )
		return ( -1 );
	idx = it - complexDirective;
	if ( _isSet[ name ] == true \
			&& _canRepeatDirective[ name ] == false )
		throw std::logic_error( "\"" + name + "\" directive is duplicate" );
	return ( idx );
}

//root <path>
void	ServerParser::parseRoot( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "root" ) ) );
	this->_rootDir = args[ 0 ];
}

//location <path>
void	ServerParser::parseLocation( std::string head, std::string body )
{
	std::string		path;
	StringVector	args;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() != 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "location" ) ) );
	args.erase( args.begin() );
	path = args.front();
	_locations.push_back( Location( path, body ) );
}

//listen
void	ServerParser::parseListen( std::string body )
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
	sep = args[ 0 ].find( ":" );
	if ( sep != std::string::npos )
		port = parsePort( args[ 0 ].substr( sep + 1, std::string::npos ), ret );
	if ( ret < 0 )
		throw std::logic_error( parseListenStrError( ret, args[ 0 ] ) );
	host = parseHost( args[ 0 ].substr( 0, sep ), ret );
	if ( ret < 0 )
		throw std::logic_error( parseListenStrError( ret, args[ 0 ] ) );
	if ( checkAvailableHostPort( host, port ) )
		throw std::logic_error( "host not found \"" + host + "\"" );
	this->_host = host;
	this->_port = std::atoi( port.c_str() );
}

//server_name {list of server names}
void	ServerParser::parseServerNames( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server_name" ) ) );
	this->_serverNames.insert( this->_serverNames.end(), args.begin(), args.end() );
}

//error_page [ code - uri ]
//code: value in range 300 - 599
//uri: internal redirect to 'uri' with client request method changed to 'GET'
void	ServerParser::parseErrorPage( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() < 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "error_page" ) ) );
	fillErrorPages( args );
}

//client_max_body_size <size>
void	ServerParser::parseClientMaxBodySize( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	try
	{
		this->_clientMaxBodySize = parseSize( args[ 0 ] );
	}
	catch ( const std::logic_error& e )
	{
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "client_max_body_size" ), args[ 0 ] ) );
	}
}
