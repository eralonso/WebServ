/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:10:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/26 19:30:29 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERPARSER_HPP_
# define _SERVERPARSER_HPP_

# include <string>
# include <limits>
# include <algorithm>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include <ServerConfig.hpp>
# include <TreeSplit.hpp>
# include <Utils.hpp>

# define SIZE_SIMPLE_OPTIONS 5
# define SIZE_COMPLEX_OPTIONS 1

# define PARSE_LISTEN_ERRORS_SIZE 3

# define IP_VALID_CHARS "0123456789."

typedef std::string	simpleDirectiveArray[ SIZE_SIMPLE_OPTIONS ];
typedef std::string	complexDirectiveArray[ SIZE_COMPLEX_OPTIONS ];

class ServerParser: public ServerConfig
{
private:
	typedef void ( ServerParser::*t_parseSimpleDirective )( std::string );
	typedef void ( ServerParser::*t_parseComplexDirective )( std::string, std::string );
	typedef t_parseSimpleDirective	t_parseSimpleDirectiveArray[ SIZE_SIMPLE_OPTIONS ];
	typedef t_parseComplexDirective	t_parseComplexDirectiveArray[ SIZE_COMPLEX_OPTIONS ];
private:
	void	parseDirective( std::string head, std::string body );
	int		isSimpleDirective( std::string head );
	int		isComplexDirective( std::string head );
private:
	//error_page
	void			fillErrorPages( StringVector args );
	int				parseErrorCode( std::string code );
	//client_max_body_size
	long			getMeasureLimit( int unit );
	int				parseMeasure( std::string number );
	long			parseSize( std::string number );
	//listen
	std::string		parseListenStrError( int ret, std::string aux );
	std::string		parseHost( std::string arg, int& ret );
	std::string		decompressIp( std::string ip );
	std::string		decompressBytes( std::string compressed, size_t pos, size_t size );
	unsigned int	getMaskLimit( size_t octetPos );
	bool			checkValidIp( std::string ip );
	bool			checkValidRangeIpMask( std::string num, size_t pos, size_t size );
	bool			checkSyntaxIp( std::string ip );
	std::string		parsePort( std::string arg, int& ret );
	bool			isValidPort( std::string port );
	int				checkAvailableHostPort( std::string host, std::string port );
private:
	void	parseRoot( std::string body );
	void	parseLocation( std::string head, std::string body );
	void	parseListen( std::string body );
	void	parseServerNames( std::string body );
	void	parseErrorPage( std::string body );
	void	parseClientMaxBodySize( std::string body );
public:
	ServerParser( std::string configs );
	~ServerParser( void );
	ServerParser( const ServerParser& b );
	ServerParser&	operator=( const ServerParser& b );
};

#endif
