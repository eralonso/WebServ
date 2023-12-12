/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/07 15:14:46 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>

# define SERVER_SIZE_DIRECTIVES 7

class Directives;

class Server
{
	//Friends
	friend class DirectivesParser;
private:
	Directives	*_directives;
public:
	Server( void );
	Server( const Server& s );
	~Server( void );
	Server&	operator=( const Server& s );
private:
	static std::string 		_allowedDirectivesAux[ SERVER_SIZE_DIRECTIVES + 1 ];
public:
	static ConstStringVector	allowedDirectives;
public:
	Location	getLocationAtPath( std::string path ) const;
	std::string	getErrorPageWithCode( unsigned int code ) const;
};

#endif
