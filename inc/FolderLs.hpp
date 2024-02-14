/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FolderLs.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:44:16 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 12:22:45 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FOLDERSLS_HPP_
# define _FOLDERSLS_HPP_

# include <cstdio>
# include <dirent.h>

# include <string>
# include <map>
# include <set>

# include <Utils.hpp>
# include <ConfigUtils.hpp>
# include <Router.hpp>

class LsEntry
{
    public:
		typedef enum e_error
		{
			NONE,
			CANTOPENDIR
		}	t_error;
	private:
		std::string name;
		unsigned char type;
	public:
        LsEntry(std::string name, unsigned char type);
		~LsEntry();
		LsEntry(const LsEntry& b);
		LsEntry& operator=(const LsEntry& b);
    	bool operator<(const LsEntry& b) const;
		const std::string& getName( void ) const;
		const unsigned char& getType( void ) const;
	private:
        static std::string epochsToDate(unsigned long int epochs);
		std::string	entryType( void ) const;
        void entryInfo(std::string &cat, const std::string &path) const;
    public:
        t_error processLsEntry(std::string &cat, const std::string &path, const std::string &route) const;
};

class FolderLs
{
private:
	FolderLs( void );
	~FolderLs( void );
	FolderLs(const FolderLs&);
	FolderLs& operator=(const FolderLs&);
public:
	typedef LsEntry::t_error t_error;
	typedef std::set<LsEntry> DirSet;
	typedef DirSet::const_iterator	DirSetIter;
public:
	static std::string		recLen( unsigned char input );
    static t_error			formatLs(std::string &res, \
									DirSet& dirMap, \
                            		const std::string &path,
                            		const std::string &route);
    static t_error			getLs(std::string &res, const std::string &path, \
                        				const std::string &route);
};

#endif
