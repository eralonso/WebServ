/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:58:06 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/24 13:26:31 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERFINDER_HPP_
# define _SERVERFINDER_HPP_

# include <TypesDefines.hpp>
# include <Server.hpp>

class ServerFinder
{
private:
	ServerFinder( void );
	~ServerFinder( void );
public:
	static const Server	*find( const ServersVector& servers, std::string host, \
								std::string port );
	static const Server	*find( const Request& request );
	static const Server	*getStrongServerMatch( const ServersVector& servers, \
	   											std::string host, \
												std::string	port );
	static const Server	*getWeakServerMatch( const ServersVector& servers, \
	   											std::string host, \
												std::string	port );
};

#endif
