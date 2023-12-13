/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/13 11:31:50 by omoreno-         ###   ########.fr       */
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
	Location *lcp;
	LocationsSet::iterator it = this->_directives->_locations.begin();
	LocationsSet::iterator ite = this->_directives->_locations.begin();
	while (it != ite)
	{
		lcp = *it;
		if (lcp->getPath() == path)
			return lcp;
		it++;
	}
	return (nullptr);
}

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}
