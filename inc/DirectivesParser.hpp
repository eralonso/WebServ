/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 11:03:56 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 19:29:35 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DIRECTIVESPARSER_HPP_
# define _DIRECTIVESPARSER_HPP_

# include <TreeSplit.hpp>
# include <Location.hpp>
# include <Defines.hpp>
# include <Directives.hpp>

# define SIZE_SIMPLE_DIRECTIVES 13
# define SIZE_COMPLEX_DIRECTIVES 2
# define SIZE_DIRECTIVES ( SIZE_SIMPLE_DIRECTIVES + SIZE_COMPLEX_DIRECTIVES )

class Directives;

class DirectivesParser
{
private:
	typedef void ( DirectivesParser::*t_parseSimpleDirective )( std::string, \
					Directives& );
	typedef void ( DirectivesParser::*t_parseComplexDirective )( std::string, \
					std::string, Directives& );
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
	static const std::string 				_DirectivesArray[ SIZE_DIRECTIVES ];
	static const std::string 				_SimpleDirectivesArray[ \
													SIZE_SIMPLE_DIRECTIVES + 1 ];
	static const std::string 				_ComplexDirectivesArray[ \
													SIZE_COMPLEX_DIRECTIVES + 1 ];
	static const std::pair< const std::string, bool >	_canRepeatDirectivePair[ \
													SIZE_DIRECTIVES + 1 ];
	static const std::map< const std::string, bool >	_canRepeatDirective;
	static t_parseSimpleDirectiveArray		_ParseSimple;
	static t_parseComplexDirectiveArray		_ParseComplex;

private:
	//parse
	void	parseDirective( std::string head, std::string body, \
					Directives& d, StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives );
	int		isSimpleDirective( std::string head, \
					StringVector allowedDirectives );
	int		isComplexDirective( std::string head, \
					StringVector allowedDirectives );

	//check
	void	checkValidDirective( std::string directive );
	void	checkValidSeparator( int type, std::string directive );
	void	checkDuplicateDirective( std::string directive, \
	 				std::map< const std::string, bool > isSet );

	//root
	void	parseRoot( std::string body, Directives& d );

	//listen
	void			parseListen( std::string body, Directives& d );

	std::string		parseListenStrError( int ret, std::string aux );
	std::string		parseHost( std::string arg, int& ret );
	std::string		decompressIp( std::string ip );
	std::string		decompressBytes( std::string compressed, \
								size_t pos, size_t size );
	unsigned int	getMaskLimit( size_t octetPos );
	bool			checkValidIp( std::string ip );
	bool			checkValidRangeIpMask( std::string num, \
								size_t pos, size_t size );
	bool			checkSyntaxIp( std::string ip );
	std::string		parsePort( std::string arg, int& ret );
	bool			isValidPort( std::string port );
	int				checkAvailableHostPort( std::string host, \
								std::string port );
	//server_name
	void	parseServerNames( std::string body, Directives& d );

	//error_page
	void	parseErrorPage( std::string body, Directives& d );

	void	fillErrorPages( StringVector args, Directives& d );
	int		parseErrorCode( std::string code );
	
	//client_max_body_size
	void	parseClientMaxBodySize( std::string body, Directives& d );

	long	getMeasureLimit( int unit );
	int		parseMeasure( std::string number );
	long	parseSize( std::string number );

	//index
	void	parseIndex( std::string body, Directives& d );

	//autoindex
	void	parseAutoindex( std::string body, Directives& d );

	//alias
	void	parseAlias( std::string body, Directives& d );

	//return
	void	parseReturn( std::string body, Directives& d );

	//allow_methods
	void	parseAllowMethods( std::string body, Directives& d );

	//cgi
	void	parseCgi( std::string body, Directives& d );

	//location
	void	parseLocation( std::string head, std::string body, Directives& d );

	//server
	void	parseServer( std::string head, std::string body, Directives& d );

public:
	static Directives	parseDirectives( std::string content, \
					StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives );
};

#endif
