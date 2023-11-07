/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIService.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:15:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 14:50:24 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CGIService.hpp>

CGIService::CGIService()
{
}

CGIService::CGIService(std::string name, unsigned int port, std::string url,
	std::string extension)
{
	this->name = name;
	this->port = port;
	this->url = url;
	this->extension = extension;
}

CGIService::~CGIService()
{
}

CGIService::CGIService(const CGIService& b)
{
	this->name = b.name;
	this->port = b.port;
	this->url = b.url;
	this->extension = b.extension;
}

CGIService& CGIService::operator=(const CGIService& b)
{
	this->name = b.name;
	this->port = b.port;
	this->url = b.url;
	this->extension = b.extension;
	return (*this);
}
unsigned int	CGIService::getPort(void)
{
	return port;
}
std::string		CGIService::getName(void)
{
	return name;
}
std::string		CGIService::getUrl(void)
{
	return url;
}
std::string		CGIService::getExtension(void)
{
	return extension;
}
