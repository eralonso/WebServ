/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/06 10:15:30 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>
# include <ConfigUtils.hpp>
# include <ConfigApply.hpp>
# include <EventsTarget.hpp>
# include <Sockets.hpp>

# define SERVER_SIZE_DIRECTIVES 11

class Receptionist;
class Directives;

class Server: public EventsTarget
{
	//Friends
	friend class DirectivesParser;
private:
	Directives			*_directives;
	struct sockaddr_in	addr;
	socket_t			_socketFd;
	Receptionist		*receptionist;
public:
	Server( void );
	Server( Events *bEvs );
	Server( const Server& s );
	~Server( void );
	Server&	operator=( const Server& s );
private:
	static std::string	_allowedDirectivesAux[ SERVER_SIZE_DIRECTIVES + 1 ];
public:
	static StringVector	allowedDirectives;
public:
	void						setAddr( const struct sockaddr_in& info );
	void						setSocketFd( socket_t fd );
	void						setReceptionist( Receptionist *recp );
	const struct sockaddr_in&	getAddr( void ) const;
	std::string					getIpString( void ) const;
	unsigned int				getIpNetworkOrder( void ) const;
	unsigned int				getIpHostOrder( void ) const;
	std::string					getHost( void ) const;
	int							getPort( void ) const;
	socket_t					getSocketFd( void ) const;
	bool						isSet( std::string directive ) const;
	bool						strongServerMatch( std::string host, \
									std::string port, unsigned int ip ) const;
	bool						weakServerMatch( std::string host, \
									std::string port, unsigned int ip ) const;
	Location					*getLocationAtPath( std::string path ) const;
	Directives					*getDirectives( void ) const;
	size_t						getMaxBodySize( std::string route ) const;
	size_t						getMaxBodySize( const Location *lc ) const;
	bool						getIsAllowedMethod( std::string route, \
									std::string method ) const;
	bool						getIsAllowedMethod( const Location *lc, \
									std::string method ) const;
	const std::string			getCgiBinary( std::string ext, \
									std::string route ) const;
	const std::string			getCgiBinary( std::string ext, \
									const Location *lc ) const;
	std::string					getFinalPath( const std::string path ) const;
	std::string					getFinalPath( const std::string path, \
									const Location *lc ) const;
	std::string					getFinalUploadPath( const std::string path ) const;
	std::string					getFinalUploadPath( const std::string path, \
									const Location *lc ) const;
	bool						getErrorPageWithCode( unsigned int code, \
									std::string& page, std::string path ) const;
	bool						getErrorPageWithCode( unsigned int code, \
									std::string& page, const Location *lc ) const;
	bool						isAutoindexAllow( const Location *lc ) const;
	bool						tryIndexFiles( std::string& file, std::string path, \
									const Location *lc ) const;
	bool						findReturnUri( int& uriCode, \
									std::string& uriRedirection, \
									const Location *lc ) const;
	void						setEvents( Events *bEvs );
	int							setEventRead( void );
	int							onNewClient( void );
	int							onEvent( Event &tevent );
};

#endif
