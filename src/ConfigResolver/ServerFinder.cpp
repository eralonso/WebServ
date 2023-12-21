/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:00:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/21 13:04:53 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ServerFinder.hpp>

ServerFinder::ServerFinder( void )
{
}

ServerFinder::~ServerFinder()
{
}

const Server *ServerFinder::find(const ServersVector& servers, std::string host, std::string port)
{
	ServersVector::const_iterator	it;
	ServersVector::const_iterator	ite = servers.end();
	
	for (it = servers.begin(); it != ite ; it++)
	{
		if ( it->serverMatch( host, port ) == true )
			return ( &( *it ) );
	}
	return NULL;
}
