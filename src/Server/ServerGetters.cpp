/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerGetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/14 11:24:21 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Receptionist.hpp>

Directives	*Server::getDirectives( void ) const { return ( this->_directives ); }

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
{ if ( this->_directives == NULL )
		return ( -1 );
	return ( this->_directives->getPort() );
}

socket_t	Server::getSocketFd( void ) const
{
	return ( this->_socketFd );
}

bool	Server::isSet( std::string directive ) const
{
	if ( this->_directives != NULL )
		return ( this->_directives->isSet( directive ) );
	return ( false );
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
	return ( lc );
}

size_t	Server::getMaxBodySize( const Location *loc ) const
{
	if ( loc != NULL && loc->isSet( "client_max_body_size" ) == true )
		return ( loc->getMaxBodySize() );
	if ( isSet( "client_max_body_size" ) == true )
		return ( this->_directives->getClientMaxBodySize() );
	return ( 1 << 20 );
}

bool	Server::getIsAllowedMethod( const Location *loc, std::string method ) const
{
	if ( loc != NULL && loc->isSet( "allow_methods" ) == true )
		return ( loc->getIsAllowedMethod( method ) );
	if ( isSet( "allow_methods" ) == true )
		return ( this->_directives->getIsAllowedMethod( method ) );
	return ( false );
}

bool	Server::getErrorPageWithCode( unsigned int code, std::string& page, \
										const Location *lc ) const
{
	if ( lc != NULL && lc->isSet( "error_page" ) == true )
		return ( lc->getErrorPageWithCode( code, page ) );
	if ( isSet( "error_page" ) == true )
		return ( this->_directives->getErrorPageWithCode( code, page ) );
	return ( false );
}

const std::string	Server::getCgiBinary( std::string ext, const Location *loc ) const
{
	if ( loc != NULL )
		return ( loc->getCgiBinary( ext ) );
	return ( "" );
}

std::string	Server::getFinalPath( const std::string path, \
									const Location *loc ) const
{
	std::string	fpath;

	if ( loc != NULL && loc->getFinalPath( path, fpath ) == true )
		return ( fpath );
	if ( isSet( "root" ) == true )
		return ( ConfigApply::applyRoot( path, this->_directives->getRoot() ) );
	return ( ConfigUtils::pathJoin( ".", path ) );
}

std::string	Server::getFinalUploadPath( const std::string path, \
											const Location *loc ) const
{
	std::string	fpath;

	if ( loc != NULL && loc->getFinalUploadPath( path, fpath ) == true )
		return ( fpath );
	if ( isSet( "upload_store" ) == true )
		return ( ConfigApply::applyAlias( path, loc->getPath(), \
					this->_directives->getUploadStore() ) );
	return ( ConfigUtils::pathJoin( ".", path ) );
}

bool	Server::isAutoindexAllow( const Location *lc ) const
{
	if ( lc != NULL && lc->isSet( "autoindex" ) == true )
		return ( lc->isAutoindexAllow() );
	else if ( isSet( "autoindex" ) == true )
		return ( this->_directives->getAutoindex() );
	return ( false );
}
