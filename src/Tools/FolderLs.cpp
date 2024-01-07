/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:47:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/19 17:30:02 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FolderLs.hpp>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

FolderLs::FolderLs( /* args */ ) {}

FolderLs::~FolderLs( void ) {}

std::string	FolderLs::recLen( unsigned char input )
{
	std::stringstream	st;

	st << ( unsigned int )input;
	return ( st.str() );
}

std::string	FolderLs::entryType( unsigned char input )
{
	unsigned char		val[] = { DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, \
									DT_REG, DT_LNK, DT_SOCK, DT_WHT };
	std::string			strVal[] = { "UNKNOWN", "FIFO", "CHR", "DIR", "BLK", \
									"REG", "LNK", "SOCK", "WHT" };
	unsigned char		n = sizeof( val ) / sizeof( unsigned char );
	unsigned char		i = 0;
	std::stringstream	st;

	while ( i < n )
	{
		if ( val[ i ] == input )
			return ( strVal[ i ] );
		i++;
	}
	st << "DT out of range [" << ( unsigned int )input << "]";
	return ( st.str() );
}

std::string	FolderLs::epochsToDate( unsigned long int epochs )
{
	std::stringstream	st;
	time_t				tt;
	struct tm			*timeinfo = NULL;
	
	tt = static_cast< std::time_t >( epochs );
	timeinfo = std::localtime ( &tt );
	st << std::setw( 4 ) << std::setfill( '0' ) << timeinfo->tm_year + 1900 << "/";
	st << std::setw( 2 ) << std::setfill( '0' ) << timeinfo->tm_mon + 1 << "/";
	st << std::setw( 2 ) << std::setfill( '0' ) << timeinfo->tm_mday + 1 << " ";
	st << std::setw( 2 ) << std::setfill( '0' ) << timeinfo->tm_hour << ":";
	st << std::setw( 2 ) << std::setfill( '0' ) << timeinfo->tm_min;
	return ( st.str() );
}

void	FolderLs::entryInfo( std::string& cat, const std::string& path )
{
	struct stat			statbuf;
	std::stringstream	st;

	if ( !stat( path.c_str() , &statbuf ) )
	{
		st << "<td>";
		st << ( unsigned int )statbuf.st_size;
		st << "</td>";
		st << "<td>";
		st << epochsToDate( static_cast< unsigned long int >( statbuf.st_mtime ) );
		st << "</td>";
		cat += st.str();
		return ;
	}
	cat += "<td>?</td>";
}

FolderLs::t_error	FolderLs::processLsEntry( std::string& cat, \
												struct dirent *pDirent,	\
												const std::string& path, \
												const std::string& route)
{
	std::string	newContent( pDirent->d_name );

	cat += "\t<tr>";
	cat += "<td>";
	if (route.size() == 0 || *(route.end() - 1) != '/' )
		cat += "<a href=" + route + std::string("/") + newContent + ">";
	else
		cat += "<a href=" + route + newContent + ">";
	cat += newContent + "</a>";
	cat += "</td>";
	cat += "<td>";
	cat += entryType( pDirent->d_type );
	cat += "</td>";
	entryInfo( cat, ConfigUtils::pathJoin( path, newContent ) );
	cat += "</tr>";
	cat += "\n";
	return ( NONE );
}

FolderLs::t_error	FolderLs::getLs(std::string& res, \
										const std::string& path, \
										const std::string& route)
{
	struct dirent		*pDirent;
	DIR					*pDir;
	t_error 			err = NONE;

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

