/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/22 17:22:47 by omoreno-         ###   ########.fr       */
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
	int						cmp = 0;

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
	//Log::Error( "max: " + SUtils::longToString( max ) );
	return ( lc );
}

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}

bool	Server::strongServerMatch( std::string host, std::string port, unsigned int ip ) const
{
	Directives						*directives = this->_directives;
	StringVector::const_iterator	it;
	StringVector::const_iterator	ite = directives->getServerNames().end();

	if ( weakServerMatch( host, port, ip ) == false )
		return ( false );
	// if ( directives->isSet( "server_name" ) == false )
	// 	return ( true );
	for ( it = directives->getServerNames().begin(); it != ite; it++ )
	{
		if ( *it == host )
			return ( true );
	}
	return ( false );
}

bool	Server::weakServerMatch( std::string host, std::string port, unsigned int ip ) const
{
	if ( SUtils::compareNumbersAsStrings( port, \
		SUtils::longToString( this->_directives->getPort() ) ) )
		return ( false );
	if ( DirectivesParser::checkValidIp( host ) == true )
	{
		if ( getIpString() == "0.0.0.0" || host == "0.0.0.0" )
			return ( true );
		return ( getIpString() == host );
	}
	return ( getIpNetworkOrder() == ip );
}

const std::string	Server::getCgiBinary( std::string ext, std::string route ) const
{
	Location	*loc = getLocationAtPath( route );

	if ( !loc )
		return ( "" );
	return ( loc->getCgiBinary( ext ) );
}

std::string	Server::getFinalPath( const std::string path ) const
{
	Location	*loc = getLocationAtPath( path );

	if ( !loc )
		return ( path );
	return ( loc->getFinalPath( path ) );
}

void	Server::setAddr( const struct sockaddr_in& info )
{
	this->addr = info;
}

const struct sockaddr_in&	Server::getAddr( void ) const
{
	return ( this->addr );
}

std::string	Server::getIpString( void ) const
{
	return ( Binary::decodeAddress( ntohl( this->addr.sin_addr.s_addr ) ) );
}

unsigned int	Server::getIpNetworkOrder( void ) const
{
	return ( this->addr.sin_addr.s_addr );
}

unsigned int	Server::getIpHostOrder( void ) const
{
	return ( ntohl( this->addr.sin_addr.s_addr ) );
}

std::string	Server::getHost( void ) const
{
	if ( this->_directives == NULL )
		return ( "" );
	return ( this->_directives->getHost() );
}

int	Server::getPort( void ) const
{
	if ( this->_directives == NULL )
		return ( -1 );
	return ( this->_directives->getPort() );
}
