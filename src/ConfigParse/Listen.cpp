/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:25:58 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/23 19:00:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerParser.hpp>

std::string	ServerParser::parsePort( std::string arg, int& ret )
{
	std::string	port;

	port = arg;
	ret = -1;
	if ( arg.length() == 0 || SUtils::isNum( arg ) == false \
			|| isValidPort( arg ) == false )
		return ( port );
	ret = 0;
	return ( port );
}

std::string	ServerParser::parseHost( std::string arg, int& ret )
{
	std::string	host;

	host = "0.0.0.0";
	ret = -2;
	if ( arg.length() == 0 )
		return ( host );
	host = arg;
	ret = 0;
	if ( checkValidIp( arg ) == true )
		host = decompressIp( arg );
	return ( host );
}

std::string	ServerParser::parseListenStrError( int ret, std::string aux )
{
	int														error;
	std::array< std::string, PARSE_LISTEN_ERRORS_SIZE + 1 >	errors = { \
		"Success", \
		"invalid port in \"" + aux + "\" of the \"listen\"", \
		"no host in \"" + aux + "\" of the \"listen\" directive", \
   		"host not found in \"" + aux + "\" of the \"listen\" directive" };

	error = ret < 0 ? -ret : ret;
	if ( error >= 0 && error < PARSE_LISTEN_ERRORS_SIZE )
		return ( errors[ error ] );
	return ( "Invalid error value" );
}

int	ServerParser::checkAvailableHostPort( std::string host, std::string port )
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	int				ret;

	ret = 0;
	std::memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	ret = getaddrinfo( host.c_str(), port.c_str(), &hints, &res );
	if ( !ret )
		Log::Error( "There's nothing" );
	freeaddrinfo( res );
	return ( ret );
}

bool	ServerParser::isValidPort( std::string port )
{
	return ( !( SUtils::compareNumbersAsStrings( port, \
			SUtils::longToString( std::numeric_limits< short >::max() ) ) > 0 \
			|| SUtils::compareNumbersAsStrings( port, "0" ) == 0 ) );
}

std::string	ServerParser::decompressIp( std::string ip )
{
	std::vector< std::string >	args;
	std::string					decompress;
	
	SUtils::split( args, ip, "." );
	for ( size_t i = 0; i < args.size(); i++ )
	{
		decompress += decompressBytes( args[ i ], i, args.size() );
		if ( i + 1 < args.size() )
			decompress += ".";
	}
	return ( decompress );
}

std::string	ServerParser::decompressBytes( std::string compressed, \
											size_t pos, size_t size )
{
	std::string		decompress;
	unsigned int	num;

	decompress = compressed;
	if ( pos == ( size - 1 ) )
	{
		decompress = "";
		num = std::atol( compressed.c_str() );
		for ( int i = 4 - size; i >= 0; i-- )
		{
			decompress += SUtils::longToString( ( num >> ( i * 8 ) ) & 0xFF );
			if ( i - 1 >= 0 )
				decompress += ".";
		}
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
