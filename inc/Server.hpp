/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/23 18:23:18 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>
# include <Client.hpp>

# define SERVER_SIZE_DIRECTIVES 7

class Directives;

class Server
{
	//Friends
	friend class DirectivesParser;
private:
	Directives			*_directives;
	struct sockaddr_in	addr;
public:
	Server( void );
	Server( const Server& s );
	~Server( void );
	Server&	operator=( const Server& s );
private:
	static std::string	_allowedDirectivesAux[ SERVER_SIZE_DIRECTIVES + 1 ];
public:
	static StringVector	allowedDirectives;
public:
	Location					*getLocationAtPath( std::string path ) const;
	Directives					*getDirectives( void ) const;
	std::string					getErrorPageWithCode( unsigned int code ) const;
	const std::string			getCgiBinary( std::string ext, std::string route ) const;
	bool						strongServerMatch( std::string host, \
									std::string port, unsigned int ip ) const;
	bool						weakServerMatch( std::string host, \
									std::string port, unsigned int ip ) const;
	std::string					getFinalPath( const std::string path ) const;
	void						setAddr( const struct sockaddr_in& info );
	const struct sockaddr_in&	getAddr( void ) const;
	std::string					getIpString( void ) const;
	unsigned int				getIpNetworkOrder( void ) const;
	unsigned int				getIpHostOrder( void ) const;
	std::string					getHost( void ) const;
	int							getPort( void ) const;
};

#endif
