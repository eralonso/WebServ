/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 11:03:56 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/07 15:01:33 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DIRECTIVESPARSER_HPP_
# define _DIRECTIVESPARSER_HPP_

# include <string.h>

# include <TypesDefines.hpp>
# include <TreeSplit.hpp>
# include <Location.hpp>
# include <Defines.hpp>
# include <Directives.hpp>
# include <Utils.hpp>
# include <DirectivesDefines.hpp>

class Directives;

class DirectivesParser
{
private:
	typedef void ( *t_parseSimpleDirective )( std::string, \
					Directives *);
	typedef void ( *t_parseComplexDirective )( std::string, \
					std::string, Directives *);
	typedef t_parseSimpleDirective	t_parseSimpleDirectiveArray[ \
					SIZE_SIMPLE_DIRECTIVES ];
	typedef t_parseComplexDirective	t_parseComplexDirectiveArray[ \
					SIZE_COMPLEX_DIRECTIVES ];
private:
	DirectivesParser( void );
	~DirectivesParser( void );
	DirectivesParser( const DirectivesParser& dp );
	DirectivesParser&	operator=( const DirectivesParser& dp );
private:
	//Auxiliar directives arrays
	static std::string 					_directivesListAux[ SIZE_DIRECTIVES + 1 ];
	static std::string 					_simpleDirectivesListAux[ \
												SIZE_SIMPLE_DIRECTIVES + 1 ];
	static std::string 					_complexDirectivesListAux[ \
												SIZE_COMPLEX_DIRECTIVES + 1 ];
	static StringBoolPair			_canRepeatDirectiveListAux[ \
												SIZE_DIRECTIVES + 1 ];

	//Parse array functions
	static t_parseSimpleDirectiveArray	_parseSimple;
	static t_parseComplexDirectiveArray	_parseComplex;
public:
	//Directives vectors/map
	static StringVector	directivesList;
	static StringVector	simpleDirectivesList;
	static StringVector	complexDirectivesList;
	static StringBoolMap	canRepeatDirectiveList;
private:
	//parse
	static void	parseLine( Directives *d, std::string& content, \
					StringVector allowedDirectives );
	static void	parseDirective( std::string head, std::string body, \
					Directives *d );

	//check
	static void	checkValidDirective( std::string directive, \
					StringVector allowedDirectives );
	static void	checkValidSeparator( int type, std::string directive );
	static void	checkDuplicateDirective( const std::string directive, \
	 				StringBoolMap isSet );

	//root
	static void	parseRoot( std::string body, Directives *d );

	//listen
	static void			parseListen( std::string body, Directives *d );

	static std::string	parseListenStrError( int ret, std::string aux );
	static std::string	parseHost( std::string arg, int& ret );
	static std::string	decompressIp( std::string ip );
	static std::string	decompressBytes( std::string compressed, \
							size_t pos, size_t size );
	static unsigned int	getMaskLimit( size_t octetPos );
	static bool			checkValidIp( std::string ip );
	static bool			checkValidRangeIpMask( std::string num, \
							size_t pos, size_t size );
	static bool			checkSyntaxIp( std::string ip );
	static std::string	parsePort( std::string arg, int& ret );
	static bool			isValidPort( std::string port );
	static int			checkAvailableHostPort( std::string host, \
							std::string port );
	//server_name
	static void	parseServerNames( std::string body, Directives *d );

	//error_page
	static void	parseErrorPage( std::string body, Directives *d );

	static void	fillErrorPages( StringVector args, Directives *d );
	static int	parseErrorCode( std::string code, int min, int max );
	
	//client_max_body_size
	static void	parseClientMaxBodySize( std::string body, Directives *d );

	static long	getMeasureLimit( int unit );
	static int	parseMeasure( std::string number );
	static long	parseSize( std::string number );

	//upload_store
	static void	parseUploadStore( std::string body, Directives *d );

	//index
	static void	parseIndex( std::string body, Directives *d );

	//autoindex
	static void	parseAutoindex( std::string body, Directives *d );

	//alias
	static void	parseAlias( std::string body, Directives *d );

	//return
	static void	parseReturn( std::string body, Directives *d );

	static bool	isHttpPrefix( std::string uri );

	//allow_methods
	static void	parseAllowMethods( std::string body, Directives *d );

	static int	isHttpMethod( std::string method );

	//cgi
	static void	parseCgi( std::string body, Directives *d );

	//location
	static void	parseLocation( std::string head, std::string body, Directives *d );

	//server
	static void	parseServer( std::string head, std::string body, Directives *d );
public:
	static Directives	*parseDirectives( std::string content, \
					StringVector allowedDirectives );
};

#endif
