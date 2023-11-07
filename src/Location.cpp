/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:58:05 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 15:45:56 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>
#include <TreeSplit.hpp>

Location::Location()
{
}

Location::Location(std::string head, std::string body)
{
	//TODO get attributes from get and
	//process the body to obtain more attributes
	std::string content = body;
	while (content.length() > 0)
	{
		if (head == "location")
		{
			std::string head;
			std::string body;
			if (TreeSplit::get_pair(head, body, content))
			{
				//TODO Analyze Head n Body to look for attributes and
				//set them or add them to the containers
				// path;
				// rootDir;
				// actionMask;
				// servicesCGI;
				try
				{
					CGIService svc;
					servicesCGI.push_back(svc);
				}
				catch(const std::exception& e)
				{
					;
					//TODO LogError and thow exception to inform parent to clean
				}
			}
		}
		else
		{
			;
			//TODO LogError and thow exception to inform parent to clean
		}
	}}

Location::~Location()
{
}

Location::Location(const Location& b)
{
	path = b.path;
	rootDir = b.rootDir;
	actionMask = b.actionMask;
	servicesCGI = b.servicesCGI;
}

Location& 	Location::operator=(const Location& b)
{
	path = b.path;
	rootDir = b.rootDir;
	actionMask = b.actionMask;
	servicesCGI = b.servicesCGI;
	return (*this);
}

std::string					Location::getPath(void)
{
	return path;
}

RootDir						Location::getRootDir(void)
{
	return rootDir;
}

ActionMask					Location::getActionMask(void)
{
	return actionMask;
}

std::vector<CGIService>		Location::getServicesCGI(void)
{
	return servicesCGI;
}
