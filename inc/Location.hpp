/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 19:24:36 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOCATION_HPP_
# define _LOCATION_HPP_

# include <vector>
# include <string>

# include <Directives.hpp>
# include <Defines.hpp>
# include <StringErrors.hpp>

# define LOCATION_SIZE_DIRECTIVES 9

class Location
{
private:
	std::string		_path;
	StringVector	_splitedPath;
	bool			_isDir;
	Directives		_directives;
public:
	Location( void );
	~Location( void );
	Location( const Location& lc );
	Location&	operator=( const Location& lc );
//public:
//	void	parseDirective( std::string head, std::string body );
public:
	std::string		getPath( void ) const;
	bool			isDir( void ) const;
	StringVector	getSplitedPath( void ) const;
	std::string		getRoot( void ) const;
	ActionMask		getAllowedMethods( void ) const;
	CgiVector		getServicesCGI( void ) const;
};

#endif
