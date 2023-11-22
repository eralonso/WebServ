/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 13:12:58 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERCONFIG_HPP
# define _SERVERCONFIG_HPP

# include <vector>
# include <array>
# include <map>
# include <string>

# include <TreeSplit.hpp>
# include <Location.hpp>
# include <RootDir.hpp>
# include <Utils.hpp>

#define SIZE_SERVER_OPTIONS 6


class ServerConfig
{
private:
	typedef void ( ServerConfig::*t_parse )( std::string );
private:
	std::vector< unsigned int >				ports;
	std::string								address;
	std::vector< Location >					locations;
	RootDir									rootDir;
	std::string								serverName;
	unsigned int							clientMaxBodySize;
	std::map< unsigned int, std::string >	errorPages;
private:
	void	parseRoot( std::string body );
	void	parseLocation( std::string body );
	void	parseListen( std::string body );
	void	parseServerName( std::string body );
	void	parseErrorPage( std::string body );
	void	parseClientMaxBodySize( std::string body );
public:
	ServerConfig( void );
	ServerConfig( std::string head, std::string body );
	~ServerConfig( void );
	ServerConfig( const ServerConfig& b );
	ServerConfig& operator=( const ServerConfig& b );
};

#endif
