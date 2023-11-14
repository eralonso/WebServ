/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:10:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/14 16:18:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERPARSER_HPP_
# define _SERVERPARSER_HPP_

# include <string>

# include <ServerConfig.hpp>

class ServerParser: public ServerConfig
{
private:
	typedef void ( ServerParser::*t_parse )( std::string );
private:
	void	parseRoot( std::string body );
	void	parseLocation( std::string body );
	void	parseListen( std::string body );
	void	parseServerName( std::string body );
	void	parseErrorPage( std::string body );
	void	parseClientMaxBodySize( std::string body );
public:
	ServerParser( void );
	ServerParser( std::string configs );
	~ServerParser( void );
	ServerParser( const ServerParser& b );
	ServerParser&	operator=( const ServerParser& b );
public:
	ServerConfig	getServerConfig( void ) const;
};

#endif
