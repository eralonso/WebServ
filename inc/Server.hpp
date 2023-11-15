/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 13:17:35 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <ServerConfig.hpp>

class Server: public ServerConfig
{
public:
	Server( void );
	Server( const Server& s );
	~Server( void );
	Server&	operator=( const Server& s );
public:
	Location&		getLocationAtPath( std::string path ) const;
	std::string&	getErrorPageWithCode( unsigned int code ) const;
};

//# define SIZE_SERVER_OPTIONS 6

//typedef std::vector< unsigned int >				PortsVector;
//typedef std::map< unsigned int, std::string >	ErrorPagesMap;
//typedef std::vector< Location >					LocationsVector;
//
//class ServerConfig:
//{
//protected:
//	PortsVector		ports;
//	std::string		address;
//	LocationsVector	locations;
//	std::string		rootDir;
//	std::string		serverName;
//	unsigned int	clientMaxBodySize;
//	ErrorPagesMap	errorPages;
//public:
//	ServerConfig( void );
//	ServerConfig( const ServerConfig& b );
//	virtual ~ServerConfig( void );
//	ServerConfig&	operator=( const ServerConfig& serverConfig );
//public:
//	PortsVector&		getPorts( void ) const;
//	std::string&		getHost( void ) const;
//	LocationsVector&	getLocations( void ) const;
//	std::string&		getRoot( void ) const;
//	std::string&		getServerName( void ) const;
//	unsigned int		getClientMaxBodySize( void ) const;
//	ErrorPagesMap&		getErrorPages( void ) const;
//};

//class Server: public ServerConfig
//{
//private:
//	typedef void ( Server::*t_parse )( std::string );
//protected:
//	std::vector< unsigned int >				ports;
//	std::string								address;
//	std::vector< Location >					locations;
//	RootDir									rootDir;
//	std::string								serverName;
//	unsigned int							clientMaxBodySize;
//	std::map< unsigned int, std::string >	errorPages;
//private:
//	void	parseRoot( std::string body );
//	void	parseLocation( std::string body );
//	void	parseListen( std::string body );
//	void	parseServerName( std::string body );
//	void	parseErrorPage( std::string body );
//	void	parseClientMaxBodySize( std::string body );
//public:
//	Server( void );
//	Server( std::string head, std::string body );
//	~Server( void );
//	Server( const Server& b );
//	Server&	operator=( const Server& b );
//};

#endif
