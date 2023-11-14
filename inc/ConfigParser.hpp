/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:49:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:04:52 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

# include <vector>
# include <string>
# include <iostream>
# include <fstream>

# include <ServerConfig.hpp>
# include <TreeSplit.hpp>
# include <Utils.hpp>

class ConfigParser
{
private:
	std::vector< ServerConfig >	serversConfig;
	std::vector< std::string >&	split( std::vector< std::string >& v, \
									std::string strArr,	\
									std::string delimiter );
public:
	ConfigParser( int argc, char **argv );
	~ConfigParser( void );
	std::vector< ServerConfig >& getServersConfig( void );
};

#endif
