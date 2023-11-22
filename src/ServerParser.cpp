/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 12:43:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

ServerParser::ServerParser( std::string options ): ServerConfig()
{
	std::string content;
	std::string head;
	std::string body;

	Log::Info( "Starting server configuration");
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
	Log::Info( "[ Config ] root: " + this->_rootDir );
	Log::Info( "[ Config ] listen: " );
	Log::Info( "[ Config ] location: " );
	Log::Info( "[ Config ] server_name: " \
			+ STLUtils::vectorToString< StringVector >( \
				this->_serverNames.begin(), this->_serverNames.end() ) );
	Log::Info( "[ Config ] error_page: " \
			+ STLUtils::mapToString< ErrorPagesMap >( \
				this->_errorPages.begin(), this->_errorPages.end() ) );
	Log::Info( "[ Config ] client_max_body_size: " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	std::cout << std::endl;
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
	std::string						name;
	std::string						args;
	simpleDirectiveArray::iterator	it;
	simpleDirectiveArray			complexDirective = { \
										"location" };

	TreeSplit::splitOnceBySpace( head, name, args );
	it = std::find( complexDirective.begin(), complexDirective.end(), name );
	if ( it == complexDirective.end() )
		return ( -1 );
	return ( it - complexDirective.begin() );
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
	( void )body;
	( void )head;
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
	sep = args[ 0 ].find( ":" );
	if ( sep != std::string::npos )
		port = parsePort( args[ 0 ].substr( sep + 1, std::string::npos ), ret );
	if ( ret < 0 )
		throw std::logic_error( parseListenStrError( ret, args[ 0 ] ) );
	host = parseHost( args[ 0 ].substr( 0, sep ), ret );
	if ( ret < 0 )
		throw std::logic_error( parseListenStrError( ret, args[ 0 ] ) );

	//host = parseHost( args[ 0 ] );
	//port = parsePort( args[ 0 ] );
	//sep = args[ 0 ].find( ":" );
	//if ( sep != std::string::npos )
	//{
	//	if ( isValidPort( args[ 0 ].substr( sep + 1, std::string::npos ) == false ) )
	//		throw std::logic_error( "invalid port in \"" \
	//				+ args[ 0 ] + "\" of the \"listen\" directive" );
	//	
	//}
	//else if ( isNum( args[ 0 ] ) == true )
	//{
	//	if ( isValidPort( args[ 0 ] ) == false )
	//		throw std::logic_error( "invalid port in \"" \
	//				+ args[ 0 ] + "\" of the \"listen\" directive" );
	//}
	//else
	//{

	//}
}

std::string	ServerParser::parseListenStrError( int ret, std::string aux )
{
	std::array< std::string, PARSE_LISTEN_ERRORS_SIZE >	errors = { \
		"invalid port in \"" + aux + "\" of the \"listen\"", \
		"no host in \"" + aux + "\" of the \"listen\" directive", \
   		"host not found in \"" + aux + "\" of the \"listen\" directive" };

	ret = ret < 0 ? -ret : ret;
	ret--;
	if ( ret >= 0 && ret < PARSE_LISTEN_ERRORS_SIZE )
		return ( errors[ ret ] );
	return ( "Success" );
}

std::string	ServerParser::parseHost( std::string arg, int& ret )
{
	std::string	host;

	host = "0.0.0.0";
	ret = -2;
	if ( arg.length() == 0 )
		return ( host );
	host = arg;
	ret = -3;
	if ( checkValidIp( arg ) == true )
		host = decompressIp( arg );
	return ( host );
}

std::string	ServerParser::decompressIp( std::string ip )
{
	std::vector< std::string >	args;
	std::string					decompress;
	
	SUtils::split( args, ip, "." );
	for ( size_t i = 0; i < args.size(); i++ )
	{
		decompress += decompressBytes( args[ 0 ], i, args.size() );
		if ( i + 1 < args.size() )
			decompress += ".";
	}
	return ( decompress );
}

std::string	ServerParser::decompressBytes( std::string compressed, size_t pos, size_t size )
{
	std::string		decompress;
	unsigned int	num;

	decompress = compressed;
	if ( pos == ( size - 1 ) )
	{
		num = std::atol( compressed.c_str() );
		//for ( size_t i = 0; i < ( 4 - pos ); i++ )
		//{
		//	
		//}
	}
	return ( decompress );
}

unsigned int	ServerParser::getMaskLimit( size_t octetPos )
{
	return ( std::numeric_limits< unsigned int >::max() >> ( octetPos * 8 ) );
}

bool	ServerParser::checkValidIp( std::string ip )
{
	std::vector< std::string >	masks;

	if ( checkSyntaxIp( ip ) == false )
		return ( false );
	SUtils::split( masks, ip, "." );
	if ( masks.size() > 4 )
		return ( false );
	for ( size_t i = 0; i < masks.size(); i++ )
	{
		if ( checkValidRangeIpMask( masks[ i ], i, masks.size() ) == false )
			return ( false );
	}
	return ( true );
}

bool	ServerParser::checkValidRangeIpMask( std::string num, size_t pos, size_t size )
{
	std::string	limit;

	limit = SUtils::longToString( ( pos == ( size - 1 ) ) ? getMaskLimit( pos ) : 0xFF );
	if ( SUtils::compareNumbersAsStrings( num, limit ) > 0 )
		return ( false );
	return ( true );
}

bool	ServerParser::checkSyntaxIp( std::string ip )
{
	bool	dot;
	size_t	pos;

	dot = false;
	pos = ip.find_first_not_of( IP_VALID_CHARS );
	if ( pos != std::string::npos || ip[ 0 ] == '.' )
		return ( false );
	pos = 0;
	while ( pos != std::string::npos )
	{
		pos = ip.find_first_of( "." );
		if ( pos != std::string::npos )
		{
			if ( ip[ pos + 1 ] == '\0' || ip[ pos + 1 ] == '.' )
				return ( false );
			ip.erase( 0, pos + 1 );
		}
	}
	return ( true );
}

std::string	ServerParser::parsePort( std::string arg, int& ret )
{
	std::string	port;

	port = "0";
	ret = -1;
	if ( arg.length() == 0 || SUtils::isNum( arg ) == false || isValidPort( arg ) == false )
		return ( port );
	ret = 0;
	return ( port );
}

bool	ServerParser::isValidPort( std::string port )
{
	return ( !( SUtils::compareNumbersAsStrings( port, \
			SUtils::longToString( std::numeric_limits< short >::max() ) ) > 0 \
			|| SUtils::compareNumbersAsStrings( port, "0" ) == 0 ) );
}

//server_name {list of server names}
void	ServerParser::parseServerNames( std::string body )
{
	SUtils::split( this->_serverNames, body, ISSPACE );
	if ( this->_serverNames.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server_name" ) ) );
}

//error_page [ code - uri ]
//code: value in range 300 - 599
//uri: internal redirect to 'uri' with client request method changed to 'GET'
void	ServerParser::parseErrorPage( std::string body )
{
	StringVector	args;
	std::string		uri;
	int				code;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() < 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "error_page" ) ) );
	uri = args.back();
	args.pop_back();
	for ( StringVector::iterator it = args.begin(); it != args.end(); it++ )
	{
		code = parseErrorCode( *it );
		if ( code < 0 )
			throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
						std::string( "error_page" ), *it ) );
		else if ( code == 0 )
			throw std::logic_error( INVALID_RANGE_DIRECTIVE( *it, \
					SUtils::longToString( MIN_ERROR_CODE ), \
					SUtils::longToString( MAX_ERROR_CODE ) ) );
		this->_errorPages[ code ] = uri;
	}
}

int	ServerParser::parseErrorCode( std::string code )
{
	if ( SUtils::isNum( code ) == false )
		return ( -1 );
	if ( SUtils::compareNumbersAsStrings( code, \
				SUtils::longToString( MIN_ERROR_CODE ) ) < 0 \
			|| SUtils::compareNumbersAsStrings( code, \
				SUtils::longToString( MAX_ERROR_CODE ) ) > 0 )
		return ( 0 );
	return ( std::atol( code.c_str() ) );
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

long	ServerParser::getMeasureLimit( int unit )
{
	return ( std::numeric_limits< long >::max() >> ( 10 * unit ) );
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
