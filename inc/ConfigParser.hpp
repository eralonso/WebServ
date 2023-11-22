/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:49:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 17:57:40 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

# include <vector>
# include <string>
# include <iostream>
# include <fstream>

# include <Server.hpp>
# include <ServerParser.hpp>
# include <TreeSplit.hpp>
# include <Utils.hpp>
# include <StringErrors.hpp>

class ConfigParser
{
private:
	std::vector< Server >	_servers;
	std::string				_fileName;
	std::string				_content;
private:
	void	checkUsage( int argc, char **argv, std::string binName );
	void	readConfig( void );
	void	parseConfigFile( void );
public:
	ConfigParser( int argc, char **argv );
	~ConfigParser( void );
	std::vector< Server > getServers( void ) const;
};

#endif
