/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:40:55 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/02 12:50:43 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>

# define SERVER_SIZE_SIMPLE_DIRECTIVES 5
# define SERVER_SIZE_COMPLEX_DIRECTIVES 1
# define SERVER_SIZE_DIRECTIVES ( SERVER_SIZE_SIMPLE_DIRECTIVES \
								+ SERVER_SIZE_COMPLEX_DIRECTIVES )

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
	static std::string 		_allowedSimpleDirectivesAux[ \
		  							SERVER_SIZE_SIMPLE_DIRECTIVES + 1 ];
	static std::string 		_allowedComplexDirectivesAux[ \
											SERVER_SIZE_COMPLEX_DIRECTIVES + 1 ];
public:
	static ConstStringVector	allowedDirectives;
	static ConstStringVector	allowedSimpleDirectives;
	static ConstStringVector	allowedComplexDirectives;
public:
	Location	getLocationAtPath( std::string path ) const;
	std::string	getErrorPageWithCode( unsigned int code ) const;
};

#endif
