/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/21 19:53:07 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ): _directives( NULL ) {}

Server::Server( const Server& s )
{
	this->_directives = NULL;
	if ( s._directives != NULL )
		this->_directives = new Directives( *s._directives );
}

Server::~Server( void )
{
	if ( this->_directives != NULL )
		delete this->_directives;
}

Server&	Server::operator=( const Server& s )
{
	if ( this != &s )
	{
		if ( this->_directives != NULL )
			delete this->_directives;
		this->_directives = NULL;
		if ( s._directives != NULL )
			this->_directives = new Directives( *s._directives );
	}
	return ( *this );
}

Directives	*Server::getDirectives( void ) const { return ( this->_directives ); }

Location*	Server::getLocationAtPath( std::string path ) const
{
	Location				*lcit = NULL;
	Location				*lc = NULL;
	LocationsSet::iterator	it = this->_directives->_locations.begin();
	LocationsSet::iterator	ite = this->_directives->_locations.end();
	int						max = 0;
	int						cmp;

	while ( it != ite )
	{
		lcit = *it;
		cmp = lcit->comparePath( path );
		if ( cmp > max )
		{
			lc = lcit;
			max = cmp;
		}
		it++;
	}
	Log::Error( "max: " + SUtils::longToString( max ) );
	return ( lc );
}
//Log::Success("Server::getLocationAtPath size: " + SUtils::longToString(this->_directives->_locations.size()));
//Log::Success("Server::getLocationAtPath comparing: " + lcp->getPath() + " with " + path);

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}

bool	Server::serverMatch( std::string host, std::string port ) const
{
	StringVector::const_iterator	it;
	StringVector::const_iterator	ite = this->_directives->getServerNames().end();

	if ( SUtils::compareNumbersAsStrings( port, \
		SUtils::longToString( this->_directives->getPort()) ) )
		return ( false );
	for ( it = this->_directives->getServerNames().begin(); it != ite; it++ )
	{
		// Log::Success(std::string("Server::serverMatch iterate: " + *it + " to locate: " + host));
		if ( *it == host )
			return ( true );
	}
	return ( false );
}

const std::string	Server::getCgiBinary( std::string ext, std::string route) const
{
	Location* loc = getLocationAtPath( route );

	if ( !loc )
		return ( "" );
	return ( loc->getCgiBinary( ext ) );
}

std::string	Server::getFinalPath( const std::string path ) const
{
	Location	*loc = getLocationAtPath( path );

	if ( !loc )
		return ( path );
	Log::Error( "siiiiii" );
	return ( loc->getFinalPath( path ) );
}
