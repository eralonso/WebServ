/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:58:05 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 14:34:49 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>

Location::Location(/* args */)
{
}

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
