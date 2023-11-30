/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:50:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 12:51:44 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DIRECTIVES_HPP_
# define _DIRECTIVES_HPP_

# include <map>
# include <limits>
# include <string>
# include <algorithm>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include <Defines.hpp>
# include <Utils.hpp>
# include <ServerConfig.hpp>
# include <TreeSplit.hpp>

# include <DirectivesParser.hpp>

# define PARSE_LISTEN_ERRORS_SIZE 3

# define IP_VALID_CHARS "0123456789."

typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::vector< Location >					LocationsVector;
typedef std::vector< Server >					ServersVector;
typedef std::pair< std::string, int >			ListenPair;

class Directives
{
	//Friend classes
	friend class Server;
	friend class Location;
	friend class DirectivesParser;
private:
	ServersVector	_servers;
	int				_port;
	std::string		_host;
	LocationsVector	_locations;
	std::string		_rootDir;
	StringVector	_serverNames;
	long			_clientMaxBodySize;
	ErrorPagesMap	_errorPages;
public:
	Directives( void );
	Directives( const Directives& s );
	~Directives( void );
	Directives&	operator=( const Directives& d );
public:
	int				getPort( void ) const;
	std::string		getHost( void ) const;
	LocationsVector	getLocations( void ) const;
	ServersVector	getServers( void ) const;
	std::string		getRoot( void ) const;
	StringVector	getServerNames( void ) const;
	unsigned int	getClientMaxBodySize( void ) const;
	ErrorPagesMap	getErrorPages( void ) const;
};

class Directives
{
private:
	Directives( void );
	~Directives( void );
	Directives( const Directives& d );
	Directives&	operator=( const Directives& d );
private:
	//error_page
	static ErrorPagesMap	fillErrorPages( StringVector args );
	static int				parseErrorCode( std::string code );
	
	//client_max_body_size
	static long				getMeasureLimit( int unit );
	static int				parseMeasure( std::string number );
	static long				parseSize( std::string number );

	//listen
	static std::string		parseListenStrError( int ret, std::string aux );
	static std::string		parseHost( std::string arg, int& ret );
	static std::string		decompressIp( std::string ip );
	static std::string		decompressBytes( std::string compressed, size_t pos, size_t size );
	static unsigned int		getMaskLimit( size_t octetPos );
	static bool				checkValidIp( std::string ip );
	static bool				checkValidRangeIpMask( std::string num, size_t pos, size_t size );
	static bool				checkSyntaxIp( std::string ip );
	static std::string		parsePort( std::string arg, int& ret );
	static bool				isValidPort( std::string port );
	static int				checkAvailableHostPort( std::string host, std::string port );
public:
	static int								isSimpleDirective( const std::string head, \
												const std::string *begin, const std::string *end );
	static int								isComplexDirective( const std::string head, \
												const std::string *begin, const std::string *end );
	static std::string						parseRoot( std::string body );
	static Location							parseLocation( std::string head, std::string body );
	static std::pair< std::string, int >	parseListen( std::string body );
	static StringVector						parseServerNames( std::string body );
	static ErrorPagesMap					parseErrorPage( std::string body );
	static long								parseClientMaxBodySize( std::string body );
};

#endif
