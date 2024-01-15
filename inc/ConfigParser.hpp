/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:49:11 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/15 09:04:35 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

# include <string>
# include <iostream>
# include <fstream>

# include <TypesDefines.hpp>
# include <Directives.hpp>
# include <Utils.hpp>

class ConfigParser
{
private:
	Directives	*_directives;
	std::string	_fileName;
	std::string	_content;
private:
	void	checkUsage( int argc, char **argv, std::string binName );
	void	readConfig( void );
public:
	ConfigParser( int argc, char **argv );
	~ConfigParser( void );
	ServersVector& getServers( void ) const;
};

#endif
