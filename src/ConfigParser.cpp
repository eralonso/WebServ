/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 16:48:49 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Utils.hpp"
#include <string>
#include <iostream>
#include <fstream>

std::vector<std::string>& ConfigParser::split(std::vector<std::string>& v,
	std::string strArr,
	std::string delimiter)
{
	std::string	temp;
    int starting = 0;
    int ending = strArr.find(delimiter);
    while (ending!=-1) {
		temp = strArr.substr(starting, ending-starting);
		if (temp.length()> 0)
        	v.push_back(temp);
        starting = ending + delimiter.size();
        ending = strArr.find(delimiter, starting);
    }
	temp = strArr.substr(starting, ending-starting);
	if (temp.length()> 0)
		v.push_back(temp);
	return (v);
}

ConfigParser::ConfigParser(int argc, char **argv)
{
	std::ifstream	myfile;
	std::string		line;
	std::string		content;

	if (argc != 2)
	{
		Log::Error("Usage: " + std::string(argv[0]) + " <config_file>");
		return ;
	}
	myfile.open(argv[1], std::ios_base::in);
	if (!myfile.is_open())
	{
		Log::Error("Invalid file: " + std::string(argv[1]));
		return ;
	}
	while (std::getline (myfile, line))
		content += line + "\n";
	myfile.close();
	
	//procesar content
}

ConfigParser::~ConfigParser()
{
}

std::vector<ServerConfig>& ConfigParser::getServersConfig(void)
{
	return (serversConfig);
}
