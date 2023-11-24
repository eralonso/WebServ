/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:58:05 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/24 13:58:12 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>
#include <TreeSplit.hpp>

Location::Location( void ) {}

Location::Location( std::string path, std::string rest )
{
	std::string head;
	std::string body;
	std::string content( rest );

	this->_path = path;
	while ( content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, content ) )
		{
		}
		else if ( content.length() > 0 )
			throw std::logic_error( "Unxpected \"}\"" );
		head.clear();
		body.clear();
	}
}

Location::~Location( void ) {}

Location::Location( const Location& lc )
{
	_path = lc._path;
	_rootDir = lc._rootDir;
	_actionMask = lc._actionMask;
	_servicesCGI = lc._servicesCGI;
}

Location& 	Location::operator=( const Location& lc )
{
	_path = lc._path;
	_rootDir = lc._rootDir;
	_actionMask = lc._actionMask;
	_servicesCGI = lc._servicesCGI;
	return ( *this );
}

std::string	Location::getPath( void ) const
{
	return ( this->_path );
}

StringVector	Location::getSplitedPath( void ) const
{
	return ( this->_splitedPath );
}

RootDir	Location::getRootDir( void ) const
{
	return ( this->_rootDir );
}

ActionMask	 Location::getActionMask( void ) const
{
	return ( this->_actionMask );
}

std::vector< CGIService>	Location::getServicesCGI( void ) const
{
	return ( this->_servicesCGI );
}
