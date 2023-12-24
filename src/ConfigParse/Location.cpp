/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:56:54 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 17:45:21 by eralonso         ###   ########.fr       */
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
											_isDir( lc._isDir )
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

bool	Location::operator<( const Location& lc ) const
{
	return ( this->_splitedPath.size() > lc._splitedPath.size() );
}

bool	Location::operator<( const Location* lc ) const
{
	if ( !lc )
		return ( true );
	return ( this->_splitedPath.size() > lc->_splitedPath.size() );
}

bool	Location::locationCompare( const Location *lc, const Location *lc2 )
{
	if ( !lc )
		return ( true );
	else if ( !lc2 )
		return ( false );
	if ( lc->_splitedPath.size() == lc2->_splitedPath.size() )
		return ( std::strcmp( lc->getPath().c_str(), lc2->getPath().c_str() ) );
	return ( lc->_splitedPath.size() > lc2->_splitedPath.size() );
}

bool	Location::operator==( const Location& lc ) const
{
	return ( STLUtils::cmpVector( this->_splitedPath, lc._splitedPath ) \
			&& this->_isDir == lc._isDir );
}

std::string	Location::getPath( void ) const { return ( this->_path ); }

StringVector	Location::getSplitedPath( void ) const { return ( this->_splitedPath ); }

bool	Location::isDir( void ) const { return ( this->_isDir ); }

Directives	*Location::getDirectives( void ) const { return ( this->_directives ); }

const std::string Location::getCgiBinary( std::string ext ) const
{
	const CgisMap	*map = NULL;

	if ( this->_directives )
	{
		map = &( this->_directives->getCgis() );
		if ( map )
			return ( map->getBinary( ext ) );
	}
	return ( "" );
}

bool	Location::getFinalPath( std::string path, std::string& fpath ) const
{
	Directives	*directives = NULL;

	directives = this->_directives;
	if ( directives == NULL )
		return ( false );
	if ( directives->isSet( "root" ) == true )
		fpath = ConfigApply::applyRoot( path, directives->getRoot() );
	else if ( directives->isSet( "alias" ) == true )
		fpath = ConfigApply::applyAlias( path, this->getPath(), \
										directives->getAlias() );
	else
		return ( false );
	return ( true );
}

bool	Location::getFinalUploadPath( std::string path, std::string& fpath ) const
{
	Directives	*directives = NULL;

	directives = this->_directives;
	if ( directives == NULL )
		return ( false );
	if ( directives->isSet( "upload_store" ) == true )
		fpath = ConfigApply::applyUploadStore( path, directives->getUploadStore() );
	else
		return ( false );
	return ( true );
}
