/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:47:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 16:50:40 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FolderLs.hpp>
#include <iostream>
#include <sstream>

FolderLs::FolderLs(/* args */)
{
}

FolderLs::~FolderLs()
{
}

std::string	FolderLs::recLen(uint8_t input)
{
	std::stringstream	st;
	st << (unsigned int)input;
	return (st.str());
}

std::string	FolderLs::entryType(uint8_t input)
{
	uint8_t val[] = {DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK,
		DT_REG, DT_LNK, DT_SOCK, DT_WHT};
	std::string strVal[] = {"DT_UNKNOWN", "DT_FIFO", "DT_CHR", "DT_DIR", "DT_BLK",
		"DT_REG", "DT_LNK", "DT_SOCK", "DT_WHT"};
	u_int8_t i = 0;
	u_int8_t n = sizeof(val) / sizeof(uint8_t);
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
	cat += std::string("<td>");
	cat += recLen(pDirent->d_reclen);
	cat += std::string("</td>");
	cat += std::string("</tr>");
	cat += "\n";
	// ino_t d_ino;                    /* file number of entry */
	// pDirent->d_type;              /* file type, see below */
	// pDirent->d_namlen;
	return (NONE);
}

FolderLs::t_error FolderLs::getLs(std::string& res,
	const std::string& path, const std::string& route)
{
	struct dirent		*pDirent;
	DIR					*pDir;
	t_error 			err = NONE;

	pDir = opendir (path.c_str());
	if (pDir == NULL)
		return (CANTOPENDIR);
	res = std::string("<table>\n");
	while ((pDirent = readdir(pDir)) != NULL)
		err = processLsEntry(res, pDirent, path, route);
	closedir (pDir);
	res += std::string("<table>\n");
	return (NONE);
}

