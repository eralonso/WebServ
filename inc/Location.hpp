/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/29 13:57:53 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOCATION_HPP_
# define _LOCATION_HPP_

# include <vector>
# include <string>

# include <RootDir.hpp>
# include <ActionMask.hpp>
# include <CGIService.hpp>
# include <Defines.hpp>
# include <StringErrors.hpp>

# define LOCATION_SIZE_DIRECTIVES 9

class Location
{
private:
	std::string					_path;
	StringVector				_splitedPath;
	bool						_isDir;
	std::string					_rootDir;
	ActionMask					_actionMask;
	std::vector< CGIService >	_servicesCGI;
	static const std::string	_directives[ LOCATION_SIZE_DIRECTIVES ];
public:
	Location( void );
	Location( std::string head, std::string body );
	~Location( void );
	Location( const Location& lc );
public:
	void	parseDirective( std::string head, std::string body );
public:
	Location& 					operator=( const Location& lc );
	std::string					getPath( void ) const;
	bool						isDir( void ) const;
	StringVector				getSplitedPath( void ) const;
	RootDir						getRootDir( void ) const;
	ActionMask					getActionMask( void ) const;
	std::vector< CGIService >	getServicesCGI( void ) const;
};

#endif
