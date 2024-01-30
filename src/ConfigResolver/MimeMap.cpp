/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 10:09:12 by codespace         #+#    #+#             */
/*   Updated: 2024/01/30 17:12:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <MimeMap.hpp>
#include <Utils.hpp>

const MimeMap::t_dict	MimeMap::dict[ MIME_NB ] = 
{
	{ "", "text/plain" },
	{ "html", "text/html" },
	{ "htm", "text/html"  },
	{ "txt", "text/plain" },
	{ "py", "text/plain" },
	{ "sh", "text/plain" },
	{ "css", "text/css" },
	{ "js", "text/javascript" },
	{ "png", "image/png" },
	{ "jpg","image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "gif", "image/gif" },
	{ "bmp", "image/bmp" },
	{ "webp", "image/webp" },
	{ "svg", "image/svg+xml" },
	{ "ico", "image/x-icon" },
	{ "mpeg", "audio/mpeg" },
	{ "ogg", "audio/ogg" },
	{ "vaw", "audio/wav" },
	{ "webm", "audio/webm" },
	{ "midi", "audio/midi" },
	{ "json", "application/json" },
	{ "pdf", "application/pdf" },
	{ "xml", "application/xhtml+xml" },
	{ "xhtml", "application/xhtml+xml" },
	{ "ppt", "application/vnd.mspowerpoint" },
	{ "pkcs12", "application/pkcs12" },
	{ "mpfd", "multipart/form-data" },
	{ "mpbr", "multipart/byteranges" },
	{ "", "application/octet-stream" }
};

MimeMap::MimeMap(void)
{
}

MimeMap::~MimeMap()
{
}

std::string MimeMap::getMime(std::string fileExt)
{
	size_t i = 0;

	while ( i < MIME_NB && std::string(dict[ i ].fileExt) != fileExt )
		i++;
	if ( i < MIME_NB )
		return ( std::string(dict[ i ].mime) );
	return ( std::string("text/plain") );   
}

std::string MimeMap::getFileExt(std::string mime)
{
	size_t i = 0;

	while ( i < MIME_NB && std::string(dict[ i ].mime) != mime )
		i++;
	if ( i < MIME_NB )
		return ( std::string(dict[ i ].fileExt) );
	return ( std::string(std::string(dict[ 0 ].fileExt)) );   
}
