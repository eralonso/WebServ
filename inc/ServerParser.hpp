/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:10:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/18 18:40:47 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERPARSER_HPP_
# define _SERVERPARSER_HPP_

# include <string>
# include <limits>
# include <array>
# include <algorithm>

# include <ServerConfig.hpp>
# include <TreeSplit.hpp>
# include <Utils.hpp>

# define SIZE_SIMPLE_OPTIONS 5
# define SIZE_COMPLEX_OPTIONS 1

typedef std::array< std::string, SIZE_SIMPLE_OPTIONS >	simpleDirectiveArray;
typedef std::array< std::string, SIZE_COMPLEX_OPTIONS >	complexDirectiveArray;

class ServerParser: public ServerConfig
{
private:
	typedef void ( ServerParser::*t_parseSimpleDirective )( std::string );
	typedef void ( ServerParser::*t_parseComplexDirective )( std::string, std::string );
	typedef std::array< t_parseSimpleDirective, SIZE_SIMPLE_OPTIONS >	t_parseSimpleDirectiveArray;
	typedef std::array< t_parseComplexDirective, SIZE_COMPLEX_OPTIONS >	t_parseComplexDirectiveArray;
private:
	void	parseDirective( std::string head, std::string body );
	int		isSimpleDirective( std::string head );
	int		isComplexDirective( std::string head );
private:
	int		parseErrorCode( std::string code );
	long	getMeasureLimit( int unit );
	int		parseMeasure( std::string number );
	long	parseSize( std::string number );
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
