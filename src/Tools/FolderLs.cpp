/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:47:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 13:03:55 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FolderLs.hpp>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

FolderLs::FolderLs(/* args */)
{
}

FolderLs::~FolderLs()
{
}

std::string	FolderLs::recLen(unsigned char input)
{
	std::stringstream	st;
	st << (unsigned int)input;
	return (st.str());
}

std::string	FolderLs::entryType(unsigned char input)
{
	unsigned char val[] = {DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK,
		DT_REG, DT_LNK, DT_SOCK, DT_WHT};
	std::string strVal[] = {"UNKNOWN", "FIFO", "CHR", "DIR", "BLK",
		"REG", "LNK", "SOCK", "WHT"};
	unsigned char i = 0;
	unsigned char n = sizeof(val) / sizeof(unsigned char);
	while (i < n)
	{
		if (val[i] == input)
			return (strVal[i]);
		i++;
	}
	std::stringstream	st;
	st << std::string("DT out of range [") << (unsigned int)input << std::string("]");
	return (st.str());
}

std::string	FolderLs::epochsToDate(unsigned long int epochs)
{
	std::stringstream	st;
	time_t tt = static_cast<std::time_t>(epochs);
	struct tm * timeinfo;
	timeinfo = std::localtime (&tt);
	st << std::setw(4) << std::setfill('0') << timeinfo->tm_year + 1900 << "/";
	st << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "/";
	st << std::setw(2) << std::setfill('0') << timeinfo->tm_mday + 1 << " ";
	st << std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":";
	st << std::setw(2) << std::setfill('0') << timeinfo->tm_min;
	return (st.str());
}

void	FolderLs::entryInfo(std::string& cat, const std::string& path)
{
	struct stat statbuf;
	std::stringstream	st;
	if (!stat(path.c_str() , &statbuf))
	{
		st << std::string("<td>");
		st << (unsigned int)statbuf.st_size;
		st << std::string("</td>");
		st << std::string("<td>");
		st << epochsToDate(static_cast< unsigned long int >( statbuf.st_mtime ));
		st << std::string("</td>");
		cat += st.str();
		return;
	}
	cat += std::string("<td>?</td>");
}

FolderLs::t_error FolderLs::processLsEntry(std::string& cat, struct dirent *pDirent,
	const std::string& path, const std::string& route)
{
	std::string	newContent(pDirent->d_name);
	cat += std::string("\t<tr>");
	cat += std::string("<td>");
	cat += "<a href=" + route + newContent + ">";
	cat += newContent + std::string("</a>");
	cat += std::string("</td>");
	cat += std::string("<td>");
	cat += entryType(pDirent->d_type);
	cat += std::string("</td>");
	entryInfo(cat, path + newContent);
	cat += std::string("</tr>");
	cat += "\n";
	return (NONE);
}

FolderLs::t_error FolderLs::getLs(std::string& res,
	const std::string& path, const std::string& route)
{
	struct dirent		*pDirent;
	DIR					*pDir;
	t_error 			err = NONE;
//
	pDir = opendir (path.c_str());
	if (pDir == NULL)
		return (CANTOPENDIR);
	res = std::string("<table>\n");
	res += std::string("<thead>");
	res += std::string("<tr>");
	res += std::string("<th>Name</th><th>Type</th><th>Size</th><th>Last Modif.</th>");
	res += std::string("</tr>");
	res += std::string("</thead>");
	res += std::string("<tbody>");
	while ((pDirent = readdir(pDir)) != NULL)
		err = processLsEntry(res, pDirent, path, route);
	closedir (pDir);
	res += std::string("</tbody>");
	res += std::string("<table>\n");
	return (err);
}

