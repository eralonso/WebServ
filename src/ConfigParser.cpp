/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 13:16:49 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Utils.hpp"
#include <string>
#include <iostream>
#include <fstream>

ConfigParser::ConfigParser(int argc, char **argv)
{
	std::ifstream	s(argv[1]);
	if (argc != 2)
	{
		Log::Error("Usage: " + std::string(argv[0]) + "<config_file>");
		return ;
	}
	s.open(argv[1], std::ios_base::in);
	if (!s.is_open())
	{
		Log::Error("Invalid file: " + std::string(argv[0]));
		return ;
	}
	
}

ConfigParser::~ConfigParser()
{
}
