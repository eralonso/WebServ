/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:56:54 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/21 19:53:46 by eralonso         ###   ########.fr       */
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

bool	Location::operator<( const Location& lc ) const
{
	return ( this->_splitedPath.size() > lc._splitedPath.size() );
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

const std::string Location::getCgiBinary(std::string ext) const
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

int	Location::comparePath( std::string path ) const
{
	size_t			cmp = 0;
	StringVector	splited;
	size_t			size = 0;

	size = this->_splitedPath.size();
	SUtils::split( splited, path, "/" );
	if ( splited.size() < size )
		return ( -1 );
	for ( size_t i = 0; i < size; i++ )
	{
		if ( splited[ i ] != this->_splitedPath[ i ] )
			break ;
		cmp++;
	}
	return ( cmp );
}

std::string	Location::pathJoin( std::string path1, std::string path2 ) const
{
	if ( path1[ path1.length() - 1 ] == '/' )
		path1.erase( path1.length() - 1 );
	if ( path2[ 0 ] == '/' )
		path2.erase( 0 );
	return ( path1 + "/" + path2 );
}

std::string	Location::getFinalPath( std::string path ) const
{
	std::string		fPath;
	StringVector	splited;
	int				cmp;

	SUtils::split( splited, path, "/" );
	fPath = path;
	cmp = comparePath( path );
	if ( this->_directives->isSet( "root" ) == true )
		fPath = pathJoin( this->_directives->getRoot(), path );
	else if ( this->_directives->isSet( "alias" ) == true )
		fPath = pathJoin( this->_directives->getAlias(), \
					STLUtils::vectorToString< StringVector >( \
					splited.begin() + cmp, splited.end() ) );
	Log::Error( "siiiiii" );
	return ( fPath );
}
