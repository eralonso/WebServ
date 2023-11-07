/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDir.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:02:57 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 15:51:27 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <RootDir.hpp>

RootDir::RootDir(/* args */)
{
}

RootDir::RootDir(std::string head, std::string body)
{
	//TODO Analyze head and assess body empty
	(void)head;
	if (body.length() > 0)
	{
		//TODO LoggError and throw to inform parent to clean
	}
}

RootDir::RootDir(std::string path)
{
	this->path = path;
}

RootDir::~RootDir()
{
}

RootDir::RootDir(const RootDir& b)
{
	path = b.path;
}

RootDir& 		RootDir::operator=(const RootDir& b)
{
	path = b.path;
	return *this;
}
