/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/08 19:36:10 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>
#include <TreeSplit.hpp>

#define SIZE_SERVER_OPTIONS 6

ServerConfig::ServerConfig( void )
{
}

ServerConfig::ServerConfig(std::string server, std::string options)
{
	//TODO get attributes from get and
	//process the body to obtain more attributes
	std::string content;
	std::string head;
	std::string body;
	std::array< std::string, SIZE_SERVER_OPTIONS >::iterator	it;
	std::array< std::string, SIZE_SERVER_OPTIONS >	availablesOptions = { \
										"root", "location", \
										"listen", "server_name", \
										"error_page", "client_max_body_size" };

	content = options;
	if (server != "server")
		throw std::logic_error( "Invalid option" );
	while (content.length() > 0)
	{
		head = "";
		body = "";
		if (TreeSplit::get_pair(head, body, content))
		{
			it = std::find( availablesOptions.begin(), availablesOptions.end(), head );
			if ( it == availablesOptions.end() )
				throw std::logic_error( "Unavailable server option" );
			//TODO Analyze Head n Body to look for attributes and
			//set them or add them to the containers
			// ports;
			// locations;
			// rootDir;
			try
			{
				Location lc;
				locations.push_back(lc);
			}
			catch(const std::exception& e)
			{
				;
				//TODO LogError and thow exception to inform parent to clean
			}
		}
	}
}

ServerConfig::~ServerConfig( void )
{
}

ServerConfig::ServerConfig(const ServerConfig& b)
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& b)
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
	return (*this);
}
