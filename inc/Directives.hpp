/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:50:07 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/23 11:58:57 by eralonso         ###   ########.fr       */
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
# include <CgisMap.hpp>

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
	std::string			_uploadStore;
	StringVector		_index;
	bool				_autoindex;
	std::string			_alias;
	ReturnPair			_return;
	ActionMask			_allowMethods;
	CgisMap				_cgis;
	ServersVector		_servers;
	LocationsSet		_locations;
	StringBoolMap		_isSet;
	bool				_isEmpty;
public:
	Directives( void );
	Directives( const Directives& s );
	~Directives( void );
	Directives&	operator=( const Directives& d );
private:
	void	deleteLocationsSet( void );
	void	copyLocationsSet( const LocationsSet& locations );
public:
	void					print( void ) const;
	std::string				getRoot( void ) const;
	int						getPort( void ) const;
	std::string				getHost( void ) const;
	const StringVector&		getServerNames( void ) const;
	const ErrorPagesMap&	getErrorPages( void ) const;
	unsigned int			getClientMaxBodySize( void ) const;
	std::string				getUploadStore( void ) const;
	const StringVector&		getIndex( void ) const;
	bool					getAutoindex( void ) const;
	std::string				getAlias( void ) const;
	const ReturnPair&		getReturn( void ) const;
	const ActionMask&		getAllowMethods( void ) const;
	const CgisMap&			getCgis( void ) const;
	const ServersVector&	getServers( void ) const;
	const LocationsSet&		getLocations( void ) const;
	const StringBoolMap&	getIsSet( void ) const;
	bool					isEmpty( void ) const;
	bool					isSet( const std::string& key );
};

#endif
