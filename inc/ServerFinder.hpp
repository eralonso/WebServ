/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:58:06 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/02 15:40:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERFINDER_HPP_
# define _SERVERFINDER_HPP_

# include <TypesDefines.hpp>
# include <Server.hpp>
# include <Clients.hpp>

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
