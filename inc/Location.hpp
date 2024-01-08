/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/02 16:00:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOCATION_HPP_
# define _LOCATION_HPP_

# include <vector>
# include <string>

# include <TypesDefines.hpp>
# include <Directives.hpp>
# include <Defines.hpp>
# include <StringErrors.hpp>
# include <ConfigUtils.hpp>
# include <ConfigApply.hpp>

# define LOCATION_SIZE_DIRECTIVES 10

class Location
{
	//Friends
	friend class DirectivesParser;
private:
	std::string		_path;
	StringVector	_splitedPath;
	bool			_isDir;
	Directives		*_directives;
private:
	static std::string	_allowedDirectivesAux[ LOCATION_SIZE_DIRECTIVES + 1 ];
public:
	static StringVector	allowedDirectives;
public:
	Location( void );
	~Location( void );
	Location( const Location& lc );
	Location&	operator=( const Location& lc );
private:
	void	deleteLocationsSet( void );
	void	copyLocationsSet( LocationsSet& locations );
public:
	bool				operator<( const Location& lc ) const;
	bool				operator<( const Location* lc ) const;
	static bool			locationCompare( const Location* lc, const Location* lc2 );
	bool				operator==( const Location& lc ) const;
	std::string			getPath( void ) const;
	bool				isDir( void ) const;
	StringVector		getSplitedPath( void ) const;
	Directives			*getDirectives( void ) const;
	const std::string	getCgiBinary( std::string ext ) const;
	bool				getFinalPath( std::string path, \
							std::string& fpath ) const;
	bool				getFinalUploadPath( std::string path, \
							std::string& fpath ) const;
	size_t				getMaxBodySize( void ) const;
	bool				isSet( std::string directive ) const;
	bool				getIsAllowedMethod( std::string method ) const;
	bool				getErrorPageWithCode( unsigned int code, \
							std::string& page ) const;
	bool				isAutoindexAllow( void ) const;
	bool				tryIndexFiles( std::string& file, std::string path ) const;
	bool				findReturnUri( int& uriCode, std::string& uriRedir ) const;
};

#endif
