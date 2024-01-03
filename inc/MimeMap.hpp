/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeMap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:10:16 by codespace         #+#    #+#             */
/*   Updated: 2024/01/03 11:26:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MIMEMAP_HPP_
# define _MIMEMAP_HPP_

#include <string>
#include <vector>

# define MIME_NB 27

class MimeMap
{
public:
	typedef struct s_dict {
		const char *fileExt;
		const char *mime;
	}	t_dict;
private:
	static const t_dict                 dict[ MIME_NB ];
private:
    MimeMap( void );
    ~MimeMap();
public:
    static std::string getMime(std::string fileExt);
    static std::string getFileExt(std::string mime);
};

#endif