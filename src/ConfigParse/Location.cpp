/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:56:54 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/30 16:52:22 by codespace        ###   ########.fr       */
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
		return ( SUtils::strcmp( lc->getPath().c_str(), lc2->getPath().c_str() ) );
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

bool	Location::isSet( std::string directive ) const
{
	if ( this->_directives != NULL )
		return ( this->_directives->isSet( directive ) );
    return ( false );
}

const std::string Location::getCgiBinary( std::string ext ) const
{
	const CgisMap	*map = NULL;

	if ( isSet( "cgi" ) == true )
	{
		map = &( this->_directives->getCgis() );
		if ( map != NULL )
			return ( map->getBinary( ext ) );
	}
	return ( "" );
}

bool	Location::getFinalPath( std::string path, std::string& fpath ) const
{
	Directives	*directives = NULL;

	directives = this->_directives;
	if ( isSet( "root" ) == true )
		fpath = ConfigApply::applyRoot( path, directives->getRoot() );
	else if ( isSet( "alias" ) == true )
		fpath = ConfigApply::applyAlias( path, this->getPath(), \
										directives->getAlias() );
	else
		return ( false );
	return ( true );
}

bool	Location::getFinalUploadPath( std::string path, std::string& fpath ) const
{
	if ( isSet( "upload_store" ) == true )
		fpath = ConfigApply::applyAlias( path, getPath(), \
					this->_directives->getUploadStore() );
	else
		return ( false );
	return ( true );
}

size_t	Location::getMaxBodySize( void ) const
{
	if ( isSet( "client_max_body_size" ) == true )
		return ( this->_directives->getClientMaxBodySize() );
    return ( 1 << 20 );
}

bool	Location::getIsAllowedMethod( std::string method ) const
{
	if ( isSet( "allow_methods" ) == true )
		return ( this->_directives->getIsAllowedMethod( method ) );
	return ( false );
}

bool	Location::getErrorPageWithCode( unsigned int code, std::string& page ) const
{
	if ( isSet( "error_page" ) == true )
		return ( this->_directives->getErrorPageWithCode( code, page ) );
	return ( false );
}

bool	Location::isAutoindexAllow( void ) const
{
	if ( isSet( "autoindex" ) == true )
		return ( this->_directives->getAutoindex() );
	return ( false );
}

bool	Location::tryIndexFiles( std::string& file, std::string path ) const
{
	if ( isSet( "index" ) == true )
		return ( this->_directives->tryIndexFiles( file, path ) );
	return ( false );
}

bool	Location::findReturnUri( int& uriCode, std::string& uriRedir ) const
{
	if ( isSet( "return" ) == true )
		return ( this->_directives->findReturnUri( uriCode, uriRedir ) );
	return ( false );
}
