/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/13 12:28:43 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <Receptionist.hpp>

Server::Server( void ): EventsTarget( NULL ), _directives( NULL ), \
							_socketFd( 0 ), receptionist( NULL ) {}

Server::Server( const Server& s ): EventsTarget( s.evs )
{
	this->_directives = NULL;
	if ( s._directives != NULL )
	{
		this->_directives = new Directives( *s._directives );
		this->_socketFd = s._socketFd;
		this->receptionist = s.receptionist;
	}
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
		this->_socketFd = s._socketFd;
		this->receptionist = s.receptionist;
	}
	return ( *this );
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

bool	Server::tryIndexFiles( std::string& file, std::string path, \
								const Location *lc ) const
{
	if ( lc != NULL && lc->isSet( "index" ) == true )
		return ( lc->tryIndexFiles( file, path ) );
	else if ( isSet( "index" ) == true )
		return ( this->_directives->tryIndexFiles( file, path ) );
	return ( false );
}

bool	Server::findReturnUri( int& uriCode, std::string& uriRedirection, \
									const Location *lc ) const
{
	if ( isSet( "return" ) == true )
		return ( this->_directives->findReturnUri( uriCode, uriRedirection ) );
	else if ( lc != NULL && lc->isSet( "return" ) == true )
		return ( lc->findReturnUri( uriCode, uriRedirection ) );
	return ( false );
}
