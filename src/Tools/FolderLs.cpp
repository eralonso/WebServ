/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:47:54 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/18 17:31:38 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FolderLs.hpp>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

LsEntry::LsEntry(std::string name, unsigned char type)
{
	this->name = name;
	this->type = type;
}

LsEntry::~LsEntry()
{
}

LsEntry::LsEntry(const LsEntry &b)
{
	name = b.name;
	type = b.type;
}

LsEntry &LsEntry::operator=(const LsEntry &b)
{
	name = b.name;
	type = b.type;
	return (*this);
}

bool LsEntry::operator<(const LsEntry &b) const
{
	if (this->type < b.type)
		return true;
	if (b.type < this->type)
		return false;
    return (this->name < b.name);
}

const std::string& LsEntry::getName( void ) const
{
    return (this->name);
}

const unsigned char &LsEntry::getType( void ) const
{
	return (this->type);
}


std::string	LsEntry::epochsToDate( unsigned long int epochs )
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

std::string	LsEntry::entryType( void ) const
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
		if ( val[ i ] == type )
			return ( strVal[ i ] );
		i++;
	}
	st << "DT out of range [" << ( unsigned int )type << "]";
	return ( st.str() );
}

void	LsEntry::entryInfo( std::string& cat, const std::string& path ) const
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

LsEntry::t_error	LsEntry::processLsEntry( std::string& cat, \
												const std::string& path, \
												const std::string& route) const
{
	bool		isDirectory;
	std::string	fname;
	
	fname = ConfigUtils::pathJoin( path, name );
	isDirectory = Router::isDir( fname );
	cat += "\t<tr>";
	cat += "<td>";
	if (route.size() == 0 || *(route.end() - 1) != '/' )
		cat += "<a href=" + route + std::string("/") + name + ( isDirectory ? "/" : "" ) + ">";
	else
		cat += "<a href=" + route + name + ( isDirectory ? "/" : "" ) + ">";
	cat += name + "</a>";
	cat += "</td>";
	cat += "<td>";
	cat += entryType();
	cat += "</td>";
	entryInfo( cat, ConfigUtils::pathJoin( path, name ) );
	cat += "</tr>";
	cat += "\n";
	return ( NONE );
}

FolderLs::FolderLs( void ) {}

FolderLs::~FolderLs( void ) {}

FolderLs::FolderLs(const FolderLs &){}

FolderLs &FolderLs::operator=(const FolderLs &)
{
    return (*this);
}

std::string	FolderLs::recLen( unsigned char input )
{
	std::stringstream	st;

	st << ( unsigned int )input;
	return ( st.str() );
}

FolderLs::t_error	FolderLs::formatLs(std::string& res,\
										DirSet& dirMap, \
										const std::string& path, \
										const std::string& route)  
{
	LsEntry::t_error err = LsEntry::NONE;
	DirSetIter	it;
	DirSetIter	ite;

	it = dirMap.begin();
	ite = dirMap.end();
	res = std::string("<table>\n");
	res += std::string("<thead>");
	res += std::string("<tr>");
	res += std::string("<th>Name</th><th>Type</th><th>Size</th><th>Last Modif.</th>");
	res += std::string("</tr>");
	res += std::string("</thead>");
	res += std::string("<tbody>");
	while (it != ite)
		err = (it++)->processLsEntry(res, path, route);
	res += std::string("</tbody>");
	res += std::string("<table>\n");
	return (err);
}

FolderLs::t_error	FolderLs::getLs(std::string& res, \
										const std::string& path, \
										const std::string& route)
{
	struct dirent		*pDirent;
	DIR					*pDir;
	DirSet			dirMap;

	pDir = opendir (path.c_str());
	if (pDir == NULL)
		return (LsEntry::CANTOPENDIR);
	while ((pDirent = readdir(pDir)) != NULL)
	{
		dirMap.insert( LsEntry(std::string(pDirent->d_name), pDirent->d_type) );
	}
	closedir (pDir);
	return formatLs(res, dirMap, path, route);
}

