/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:58:05 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 13:11:32 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>

Location::Location(/* args */)
{
}

Location::~Location()
{
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
