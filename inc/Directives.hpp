/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:50:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/04 12:26:59 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DIRECTIVES_HPP_
# define _DIRECTIVES_HPP_

# include <map>
# include <limits>
# include <string>
# include <algorithm>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include <TypesDefines.hpp>
# include <Defines.hpp>
# include <Utils.hpp>
# include <TreeSplit.hpp>
# include <CGIService.hpp>
# include <Location.hpp>
# include <Server.hpp>
# include <ActionMask.hpp>
# include <DirectivesParser.hpp>

class Directives
{
	//Friend classes
	friend class Server;
	friend class Location;
	friend class ConfigParser;
	friend class DirectivesParser;
private:
	std::string			_root;
	int					_port;
	std::string			_host;
	StringVector		_serverNames;
	ErrorPagesMap		_errorPages;
	long				_clientMaxBodySize;
	StringVector		_index;
	bool				_autoindex;
	std::string			_alias;
	ReturnPair			_return;
	ActionMask			_allowMethods;
	CgiVector			_cgi;
	ServersVector		_servers;
	LocationsSet		_locations;
	ConstStringBoolMap	_isSet;
public:
	Directives( void );
	Directives( const Directives& s );
	~Directives( void );
	Directives&	operator=( const Directives& d );
public:
	std::string		getRoot( void ) const;
	int				getPort( void ) const;
	std::string		getHost( void ) const;
	StringVector	getServerNames( void ) const;
	ErrorPagesMap	getErrorPages( void ) const;
	unsigned int	getClientMaxBodySize( void ) const;
	StringVector	getIndex( void ) const;
	bool			getAutoindex( void ) const;
	std::string		getAlias( void ) const;
	ReturnPair		getReturn( void ) const;
	ActionMask		getAllowMethods( void ) const;
	CgiVector		getCgi( void ) const;
	ServersVector	getServers( void ) const;
	LocationsSet	getLocations( void ) const;
};

#endif
