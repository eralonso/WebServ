/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:25:25 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/27 11:52:13 by eralonso         ###   ########.fr       */
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
# include <Defines.hpp>

# define MIN_ERROR_CODE 300 
# define MAX_ERROR_CODE 599 

typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::vector< Location >					LocationsVector;

class ServerConfig
{
protected:
	int				_port;
	std::string		_host;
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
	int				getPort( void ) const;
	std::string		getHost( void ) const;
	LocationsVector	getLocations( void ) const;
	std::string		getRoot( void ) const;
	StringVector	getServerNames( void ) const;
	unsigned int	getClientMaxBodySize( void ) const;
	ErrorPagesMap	getErrorPages( void ) const;
};

#endif
