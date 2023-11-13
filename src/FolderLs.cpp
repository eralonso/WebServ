/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:47:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 13:47:21 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FolderLs.hpp>
#include <iostream>

FolderLs::FolderLs(/* args */)
{
}

FolderLs::~FolderLs()
{
}
FolderLs::t_error FolderLs::processLsEntry(std::string& cat, struct dirent *pDirent,
	const std::string& path, const std::string& route)
{
	std::string	newContent(pDirent->d_name);
	cat += "<a href=" + path + newContent + ">";
	cat += newContent + std::string("</a>");
	cat += "\n";
	// ino_t d_ino;                    /* file number of entry */
	// pDirent->d_reclen;            /* length of this record */
	// pDirent->d_type;              /* file type, see below */
	// pDirent->d_namlen;
	return (NONE);
}

FolderLs::t_error FolderLs::getLs(std::string& res,
	const std::string& path, const std::string& route)
{
	struct dirent		*pDirent;
	DIR					*pDir;
	std::string			ret("");
	t_error 			err = NONE;

	std::cout << path.c_str() << "\n";
	pDir = opendir (path.c_str());
	if (pDir == NULL)
		return (CANTOPENDIR);
	while ((pDirent = readdir(pDir)) != NULL)
		err = processLsEntry(ret, pDirent, path, route);
	closedir (pDir);
	return (err);
}

