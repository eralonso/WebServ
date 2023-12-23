/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/23 11:48:38 by eralonso         ###   ########.fr       */
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
	int					comparePath( std::string path ) const;
	std::string			getFinalPath( std::string path ) const;
	std::string			pathJoin( std::string path1, std::string path2 ) const;
};

#endif
