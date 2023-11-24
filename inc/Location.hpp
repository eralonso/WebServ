/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/24 13:55:50 by eralonso         ###   ########.fr       */
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

class Location
{
private:
	std::string				_path;
	StringVector			_splitedPath;
	RootDir					_rootDir;
	ActionMask				_actionMask;
	std::vector<CGIService>	_servicesCGI;
public:
	Location( void );
	Location( std::string head, std::string body );
	~Location( void );
	Location( const Location& b );
	Location& 					operator=( const Location& b );
	std::string					getPath( void ) const;
	StringVector				getSplitedPath( void ) const;
	RootDir						getRootDir( void ) const;
	ActionMask					getActionMask( void ) const;
	std::vector< CGIService >	getServicesCGI( void ) const;
};

#endif
