/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:00:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/20 18:21:33 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ServerFinder.hpp>

ServerFinder::ServerFinder( void )
{
}

ServerFinder::~ServerFinder()
{
}

Server *ServerFinder::find(ServersVector servers, std::string host, std::string port)
{
	ServersVector::iterator	it;
	ServersVector::iterator	ite = servers.end();
	
	for (it = servers.begin(); it != ite ; it++)
	{
		if ( it->serverMatch( host, port ) == true )
			return ( &( *it ) );
	}
	return NULL;
}
