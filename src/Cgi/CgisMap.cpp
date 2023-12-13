/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgisMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:18:09 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 12:22:14 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CgisMap.hpp>

CgisMap::CgisMap()
{
}

CgisMap::~CgisMap()
{
}

CgisMap::CgisMap(const CgisMap &): std::map<std::string, std::string>()

{
}

CgisMap &CgisMap::operator=(const CgisMap &)
{
	return *this;
}

int CgisMap::appendCgi(std::string ext, std::string binary)
{
	this->insert(std::pair<std::string, std::string>(ext, binary));
	return 1;
}

std::string CgisMap::getBinary(std::string ext)
{
	return this->operator[](ext);
}

bool CgisMap::findCgi(std::string ext, std::string& binary)
{
	std::map<std::string, std::string>::iterator it = this->find(ext);
	if (it != this->end())
	{
		binary = it->second;
		return true;
	}
	return false;
}
