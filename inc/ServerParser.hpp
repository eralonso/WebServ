/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:10:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/29 19:45:23 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERPARSER_HPP_
# define _SERVERPARSER_HPP_

# include <string>
# include <limits>
# include <algorithm>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include <ServerConfig.hpp>
# include <TreeSplit.hpp>
# include <Utils.hpp>
# include <Directives.hpp>

# define SERVER_SIZE_SIMPLE_DIRECTIVES 5
# define SERVER_SIZE_COMPLEX_DIRECTIVES 1
# define SERVER_SIZE_DIRECTIVES ( SERVER_SIZE_SIMPLE_DIRECTIVES + SERVER_SIZE_COMPLEX_DIRECTIVES )

# define PARSE_LISTEN_ERRORS_SIZE 3

# define IP_VALID_CHARS "0123456789."

typedef std::string	serverSimpleDirectiveArray[ SERVER_SIZE_SIMPLE_DIRECTIVES + 1 ];
typedef std::string	serverComplexDirectiveArray[ SERVER_SIZE_COMPLEX_DIRECTIVES + 1 ];

class ServerParser: public ServerConfig
{
private:
	typedef void ( Directives::*t_parseSimpleDirective )( std::string );
	typedef void ( Directives::*t_parseComplexDirective )( std::string, std::string );
	typedef t_parseSimpleDirective	t_parseSimpleDirectiveArray[ SERVER_SIZE_SIMPLE_DIRECTIVES ];
	typedef t_parseComplexDirective	t_parseComplexDirectiveArray[ SERVER_SIZE_COMPLEX_DIRECTIVES ];
private:
	static const std::string 				_serverAllowedDirectives[ \
													SERVER_SIZE_DIRECTIVES ];
	static const std::string 				_serverAllowedSimpleDirectives[ \
													SERVER_SIZE_SIMPLE_DIRECTIVES + 1 ];
	static const std::string 				_serverAllowedComplexDirectives[ \
													SERVER_SIZE_COMPLEX_DIRECTIVES + 1 ];
	static std::pair< std::string, bool >	_canRepeatDirectivePair[ SERVER_SIZE_DIRECTIVES + 1 ];
	static std::map< std::string, bool >	_canRepeatDirective;
	std::map< const std::string, bool >		_isSet;
	static t_parseSimpleDirectiveArray		_serverParseSimple;
	static t_parseComplexDirectiveArray		_serverParseComplex;
private:
	void	parseDirective( std::string head, std::string body );
public:
	ServerParser( std::string configs );
	~ServerParser( void );
	ServerParser( const ServerParser& b );
	ServerParser&	operator=( const ServerParser& b );
};

#endif
