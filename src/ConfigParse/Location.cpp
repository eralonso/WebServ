/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:58:05 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/29 13:57:40 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>
#include <TreeSplit.hpp>

const std::string	Location::_directives[ LOCATION_SIZE_DIRECTIVES ] = 
{
	"root",
	"index",
	"autoindex",
	"error_page",
	"client_max_body_size",
	"alias",
	"rewrite",
	"allow_methods",
	"cgi"
};

Location::Location( void ): _isDir( false ) {}

Location::Location( std::string path, std::string rest ): _isDir( false )
{
	std::string head;
	std::string body;
	std::string content( rest );

	this->_path = path;
	SUtils::split( this->_splitedPath, path, "/" );
	if ( path.back() == '/' )
		this->_isDir = true;
	while ( content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, content ) )
			parseDirective( head, body );
		else if ( content.length() > 0 )
			throw std::logic_error( "Unxpected \"}\"" );
		head.clear();
		body.clear();
	}
}

Location::~Location( void ) {}

Location::Location( const Location& lc ): _path( lc._path ), _isDir(lc._isDir ), \
					_rootDir( lc._rootDir ), _actionMask( lc._actionMask ), \
					_servicesCGI( lc._servicesCGI ) {}

Location& 	Location::operator=( const Location& lc )
{
	_path = lc._path;
	_isDir = lc._isDir;
	_rootDir = lc._rootDir;
	_actionMask = lc._actionMask;
	_servicesCGI = lc._servicesCGI;
	return ( *this );
}

void	Location::parseDirective( std::string head, std::string body )
{
	int								idx;
	void ( Location::*parse[ LOCATION_SIZE_DIRECTIVES ] )( std::string ) = { \
					&Location::parseRoot, &Location::parseErrorPage, \
					&Location::parseClientMaxBodySize };

	if ( ( idx = isSimpleDirective( head ) ) >= 0 )
		( this->*parseSimple[ idx ] )( body );
	else
		throw std::logic_error( UNKNOWN_DIRECTIVE( head ) );
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

bool	Location::isDir( void ) const
{
	return ( this->_isDir );
}

ActionMask	 Location::getActionMask( void ) const
{
	return ( this->_actionMask );
}

std::vector< CGIService>	Location::getServicesCGI( void ) const
{
	return ( this->_servicesCGI );
}
