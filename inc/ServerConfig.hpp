/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:25:25 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/18 18:33:51 by eralonso         ###   ########.fr       */
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

# define MIN_ERROR_CODE 300 
# define MAX_ERROR_CODE 599 

typedef std::vector< unsigned int >				PortsVector;
typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::vector< Location >					LocationsVector;
typedef std::vector< std::string >				StringVector;

class ServerConfig
{
protected:
	PortsVector		_ports;
	std::string		_address;
	LocationsVector	_locations;
	std::string		_rootDir;
	StringVector	_serverNames;
	long			_clientMaxBodySize;
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
	StringVector	getServerNames( void ) const;
	unsigned int	getClientMaxBodySize( void ) const;
	ErrorPagesMap	getErrorPages( void ) const;
};

#endif
