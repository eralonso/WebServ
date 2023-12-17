/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:44:16 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 16:37:19 by eralonso         ###   ########.fr       */
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
	FolderLs( void );
	~FolderLs( void );
public:
	typedef enum e_error
	{
		NONE,
		CANTOPENDIR
	}	t_error;
public:
	static std::string	entryType( unsigned char input );
	static std::string	recLen( unsigned char input );
	static std::string	epochsToDate( unsigned long int epochs );
	static void			entryInfo( std::string& cat, const std::string& path );
	static t_error		processLsEntry( std::string& cat, \
										struct dirent *pDirent,	\
										const std::string& path, \
										const std::string& route );
	static t_error		getLs( std::string& res, const std::string& path, \
								const std::string& route);
};

#endif
