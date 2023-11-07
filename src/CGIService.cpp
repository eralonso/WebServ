/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIService.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:15:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 12:49:30 by omoreno-         ###   ########.fr       */
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
