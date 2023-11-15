/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:25:25 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 17:59:50 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERCONFIG_HPP_
# define _SERVERCONFIG_HPP_

# include <vector>
# include <map>
# include <string>

# include <Location.hpp>
# include <RootDir.hpp>
# include <StringErrors.hpp>

typedef std::vector< unsigned int >				PortsVector;
typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::vector< Location >					LocationsVector;

class ServerConfig
{
protected:
	PortsVector		_ports;
	std::string		_address;
	LocationsVector	_locations;
	std::string		_rootDir;
	std::string		_serverName;
	unsigned int	_clientMaxBodySize;
	ErrorPagesMap	_errorPages;
public:
	ServerConfig( void );
	ServerConfig( const ServerConfig& s );
	virtual ~ServerConfig( void );
	ServerConfig&	operator=( const ServerConfig& s );
public:
	PortsVector		getPorts( void ) const;
	std::string		getHost( void ) const;
	LocationsVector	getLocations( void ) const;
	std::string		getRoot( void ) const;
	std::string		getServerName( void ) const;
	unsigned int	getClientMaxBodySize( void ) const;
	ErrorPagesMap	getErrorPages( void ) const;
};

#endif
