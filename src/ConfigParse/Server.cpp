/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/02 13:11:41 by codespace        ###   ########.fr       */
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

size_t Server::getMaxBodySize(std::string route) const
{
	if (this->_directives)
	{
		Location	*loc = getLocationAtPath( route );
		
		if ( loc != NULL && loc->isSet("client_max_body_size") )
			return ( loc->getMaxBodySize() );
		return (this->_directives->getClientMaxBodySize());
	}	
    return (1 << 20);
}

Location	*Server::getLocationAtPath( std::string path ) const
{
	Location				*lcit = NULL;
	Location				*lc = NULL;
	LocationsSet::iterator	it;
	LocationsSet::iterator	ite;
	int						max = 0;
	int						cmp = 0;

	if ( this->_directives == NULL )
		return ( NULL );
	it = this->_directives->_locations.begin();
	ite = this->_directives->_locations.end();
	while ( it != ite )
	{
		lcit = *it;
		cmp = ConfigUtils::comparePathReference( path, lcit->getPath() );
		if ( cmp > max )
		{
			lc = lcit;
			max = cmp;
		}
		it++;
	}
	if ( lc == NULL && lcit != NULL && lcit->getSplitedPath().size() == 0 )
		lc = lcit;
	return ( lc );
}

std::string	Server::getErrorPageWithCode( unsigned int code ) const
{
	( void ) code;
	return ( "a" );
}

bool	Server::strongServerMatch( std::string host, std::string port, \
									unsigned int ip ) const
{
	Directives						*directives = this->_directives;
	StringVector::const_iterator	it;
	StringVector::const_iterator	ite;

	if ( directives == NULL || weakServerMatch( host, port, ip ) == false )
		return ( false );
	ite = directives->getServerNames().end();
	for ( it = directives->getServerNames().begin(); it != ite; it++ )
	{
		if ( *it == host )
			return ( true );
	}
	return ( false );
}

bool	Server::weakServerMatch( std::string host, std::string port, \
								unsigned int ip ) const
{
	if ( this->_directives == NULL )
		return ( false );
	if ( SUtils::compareNumbersAsStrings( port, \
		SUtils::longToString( this->_directives->getPort() ) ) )
		return ( false );
	if ( DirectivesParser::checkValidIp( host ) == true )
	{
		std::string IpStr = getIpString(); 
		if ( IpStr == "0.0.0.0" || host == "0.0.0.0" )
			return ( true );
		return ( IpStr == host );
	}
	unsigned int ipNwk = getIpNetworkOrder();
	return (ipNwk == 0 || ipNwk == ip );
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

const std::string	Server::getCgiBinary( std::string ext, \
											std::string route ) const
{
	Location	*loc = getLocationAtPath( route );

	if ( loc != NULL )
		return ( loc->getCgiBinary( ext ) );
	return ( "" );
}

std::string	Server::getFinalPath( const std::string path ) const
{
	Location	*loc = getLocationAtPath( path );
	std::string	fpath;

	if ( loc != NULL && loc->getFinalPath( path, fpath ) == true )
		return ( fpath );
	if ( this->_directives != NULL \
			&& this->_directives->isSet( "root" ) == true )
		return ( ConfigApply::applyRoot( path, this->_directives->getRoot() ) );
	return ( ConfigUtils::pathJoin( ".", path ) );
}

std::string	Server::getFinalUploadPath( const std::string path ) const
{
	Location	*loc = getLocationAtPath( path );
	std::string	fpath;

	if ( loc != NULL && loc->getFinalUploadPath( path, fpath ) == true )
		return ( fpath );
	if ( this->_directives != NULL \
			&& this->_directives->isSet( "upload_store" ) == true )
		return ( ConfigApply::applyUploadStore( path, \
									this->_directives->getUploadStore() ) );
	return ( ConfigUtils::pathJoin( ".", path ) );
}
