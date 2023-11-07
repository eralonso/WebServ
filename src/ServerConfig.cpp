/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 15:36:39 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>
#include <TreeSplit.hpp>

ServerConfig::ServerConfig()
{
}

ServerConfig::ServerConfig(std::string head, std::string body)
{
	//TODO get attributes from get and
	//process the body to obtain more attributes
	(void)head;
	std::string content = body;
	while (content.length() > 0)
	{
		std::string head;
		std::string body;
		if (TreeSplit::get_pair(head, body, content))
		{
			//TODO Analyze Head n Body to look for attributes and
			//set them or add them to the containers
			// ports;
			// locations;
			// rootDir;
			if (head == "server")
			{
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
			else
			{
					//TODO LogError and thow exception to inform parent to clean
			}
		}
	}
}

ServerConfig::~ServerConfig()
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
