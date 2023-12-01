/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/01 13:52:55 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>

class Directives;

class Server
{
private:
	Directives	*_directives;
public:
	Server( void );
	Server( const Server& s );
	~Server( void );
	Server&	operator=( const Server& s );
public:
	Location	getLocationAtPath( std::string path ) const;
	std::string	getErrorPageWithCode( unsigned int code ) const;
};

#endif
