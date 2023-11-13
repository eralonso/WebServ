/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:44:16 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 16:49:34 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FOLDERSLS_HPP_
# define _FOLDERSLS_HPP_
# include <stdio.h>
# include <dirent.h>
# include <string>

class FolderLs
{
private:
	FolderLs();
	~FolderLs();
public:
	typedef enum e_error
	{
		NONE,
		CANTOPENDIR
	}	t_error;
	static std::string	entryType(uint8_t input);
	static std::string	recLen(uint8_t input);
	static t_error processLsEntry(std::string& cat, struct dirent *pDirent,
		const std::string& path, const std::string& route);
	static t_error getLs(std::string& res,
		const std::string& path, const std::string& route);
};

#endif
