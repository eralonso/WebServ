/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/02 12:54:17 by eralonso         ###   ########.fr       */
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

# define LOCATION_SIZE_SIMPLE_DIRECTIVES 9
# define LOCATION_SIZE_COMPLEX_DIRECTIVES 0
# define LOCATION_SIZE_DIRECTIVES ( LOCATION_SIZE_SIMPLE_DIRECTIVES \
								+ LOCATION_SIZE_COMPLEX_DIRECTIVES )

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
	static std::string 		_allowedDirectivesAux[ LOCATION_SIZE_DIRECTIVES + 1 ];
	static std::string 		_allowedSimpleDirectivesAux[ \
											LOCATION_SIZE_SIMPLE_DIRECTIVES + 1 ];
public:
	static ConstStringVector	allowedDirectives;
	static ConstStringVector	allowedSimpleDirectives;
	static ConstStringVector	allowedComplexDirectives;
public:
	Location( void );
	~Location( void );
	Location( const Location& lc );
	Location&	operator=( const Location& lc );
public:
	std::string		getPath( void ) const;
	bool			isDir( void ) const;
	StringVector	getSplitedPath( void ) const;
	Directives		*getDirectives( void ) const;
};

#endif
