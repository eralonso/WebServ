/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/29 19:44:36 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ) {}

Directives::~Directives( void ) {}

Directives::Directives( const Directives& d )
{
	( void ) d;
}

Directives&	Directives::operator=( const Directives& d )
{
	( void ) d;
	return ( *this );
}

int	Directives::isSimpleDirective( const std::string head, const std::string *begin, \
									const std::string *end )
{
	const std::string	*it;
	int					idx;

	it = std::find( begin, end, head );
	if ( it == end )
		return ( -1 );
	idx = it - begin;
	//if ( _isSet[ head ] == true && _canRepeatDirective[ head ] == false )
	//	throw std::logic_error( "\"" + head + "\" directive is duplicate" );
	//_isSet[ simpleDirective[ idx ] ] = true;
	return ( idx );
}

int	Directives::isComplexDirective( const std::string head, const std::string *begin, \
										const std::string *end )
{
	std::string			name;
	std::string			args;
	const std::string	*it;
	int					idx;

	TreeSplit::splitOnceBySpace( head, name, args );
	it = std::find( begin, end, static_cast< const std::string >( name ) );
	if ( it == end )
		return ( -1 );
	idx = it - begin;
	//if ( _isSet[ name ] == true && _canRepeatDirective[ name ] == false )
	//	throw std::logic_error( "\"" + name + "\" directive is duplicate" );
	//_isSet[ complexDirective[ idx ] ] = true;
	return ( idx );
}

//root <path>
std::string	Directives::parseRoot( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "root" ) ) );
	return ( args[ 0 ] );
}

//location <path>
Location	Directives::parseLocation( std::string head, std::string body )
{
	std::string		path;
	StringVector	args;

	SUtils::split( args, head, ISSPACE );
	if ( args.size() != 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "location" ) ) );
	args.erase( args.begin() );
	path = args.front();
	return ( Location( path, body ) );
}

//listen
std::pair< std::string, int >	Directives::parseListen( std::string body )
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
	return ( std::make_pair< std::string, int >( host, std::atoi( port.c_str() ) ) );
}

//server_name {list of server names}
StringVector	Directives::parseServerNames( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() == 0 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "server_name" ) ) );
	return ( args );
}

//error_page [ code - uri ]
//code: value in range 300 - 599
//uri: internal redirect to 'uri' with client request method changed to 'GET'
ErrorPagesMap	Directives::parseErrorPage( std::string body )
{
	StringVector	args;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() < 2 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "error_page" ) ) );
	return ( fillErrorPages( args ) );
}

//client_max_body_size <size>
long	Directives::parseClientMaxBodySize( std::string body )
{
	StringVector	args;
	long			size;

	SUtils::split( args, body, ISSPACE );
	if ( args.size() != 1 )
		throw std::logic_error( INVALID_NUMBER_ARGUMENTS_DIRECTIVE( \
					std::string( "client_max_body_size" ) ) );
	try
	{
		size = parseSize( args[ 0 ] );
	}
	catch ( const std::logic_error& e )
	{
		throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
					std::string( "client_max_body_size" ), args[ 0 ] ) );
	}
	return ( size );
}
