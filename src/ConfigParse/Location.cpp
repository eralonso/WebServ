/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:56:54 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 12:58:14 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>

Location::Location( void ): _isDir( false ), _directives( NULL ) {}

Location::~Location( void )
{
	if ( this->_directives != NULL )
		delete this->_directives;
}

Location::Location( const Location& lc ): _path( lc._path ), \
											_splitedPath( lc._splitedPath ), \
											_isDir(lc._isDir )
{
	this->_directives = NULL;
	if ( lc._directives != NULL )
		this->_directives = new Directives( *lc._directives );
}

Location& 	Location::operator=( const Location& lc )
{
	if ( this != &lc )
	{
		this->_path = lc._path;
		this->_splitedPath = lc._splitedPath;
		this->_isDir = lc._isDir;
		if ( this->_directives != NULL )
			delete this->_directives;
		this->_directives = NULL;
		if ( lc._directives != NULL )
			this->_directives = new Directives( *lc._directives );
	}
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

bool	Location::isDir( void ) const
{
	return ( this->_isDir );
}

Directives	*Location::getDirectives( void ) const
{
	return ( this->_directives );
}
