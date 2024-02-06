/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 13:10:34 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/06 10:36:28 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

Server::Server( void ): EventsTarget( NULL ), _directives( NULL ), \
							_socketFd( 0 ), receptionist( NULL ) {}

Server::Server( const Server& s ): EventsTarget( s.evs )
{
	this->_directives = NULL;
	if ( s._directives != NULL )
	{
		this->_directives = new Directives( *s._directives );
		this->_socketFd = s.socketFd;
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

void	Server::setAddr( const struct sockaddr_in& info )
{
	this->addr = info;
}

void	Server::setSocketFd( socket_t fd )
{
	this->_socketFd = fd;
}

void	Server::setReceptionist( Receptionist *recp )
{
	this->receptionist = recp;
}

void	Server::setEvents( Events *bEvs )
{
	this->evs = bEvs;
}

int	Server::setEventRead( void )
{
	if ( this->evs )
		return ( this->evs->setEventRead( this, this->_socketFd ) );
	return ( 0 );
}

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

size_t	Server::getMaxBodySize( std::string route ) const
{
	if ( this->_directives != NULL )
	{
		const Location	*loc = getLocationAtPath( route );

		if ( loc != NULL && loc->isSet( "client_max_body_size" ) == true )
			return ( loc->getMaxBodySize() );
		return ( this->_directives->getClientMaxBodySize() );
	}	
	return ( 1 << 20 );
}

size_t	Server::getMaxBodySize( const Location *loc ) const
{
	if ( loc != NULL && loc->isSet( "client_max_body_size" ) == true )
		return ( loc->getMaxBodySize() );
	if ( isSet( "client_max_body_size" ) == true )
		return ( this->_directives->getClientMaxBodySize() );
	return ( 1 << 20 );
}

bool	Server::getIsAllowedMethod( std::string route, std::string method ) const
{
	if ( this->_directives != NULL )
	{
		const Location	*loc = getLocationAtPath( route );
		
		if ( loc != NULL && loc->isSet( "allow_methods" ) == true )
			return ( loc->getIsAllowedMethod( method ) );
		return ( this->_directives->getIsAllowedMethod( method ) );
	}	
	return ( false );
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
										std::string path ) const
{
	const Location	*lc = NULL;

	if ( this->_directives != NULL )
	{
		lc = getLocationAtPath( path );
		if ( lc != NULL && lc->isSet( "error_page" ) == true )
			return ( lc->getErrorPageWithCode( code, page ) );
		if ( isSet( "error_page" ) == true )
			return ( this->_directives->getErrorPageWithCode( code, page ) );
	}
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

const std::string	Server::getCgiBinary( std::string ext, std::string route ) const
{
	Location	*loc = getLocationAtPath( route );

	if ( loc != NULL )
		return ( loc->getCgiBinary( ext ) );
	return ( "" );
}

const std::string	Server::getCgiBinary( std::string ext, const Location *loc ) const
{
	if ( loc != NULL )
		return ( loc->getCgiBinary( ext ) );
	return ( "" );
}

std::string	Server::getFinalPath( const std::string path ) const
{
	const Location	*loc = getLocationAtPath( path );
	std::string	fpath;

	if ( loc != NULL && loc->getFinalPath( path, fpath ) == true )
		return ( fpath );
	if ( isSet( "root" ) == true )
		return ( ConfigApply::applyRoot( path, this->_directives->getRoot() ) );
	return ( ConfigUtils::pathJoin( ".", path ) );
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

std::string	Server::getFinalUploadPath( const std::string path ) const
{
	const Location	*loc = getLocationAtPath( path );
	std::string		fpath;

	if ( loc != NULL && loc->getFinalUploadPath( path, fpath ) == true )
		return ( fpath );
	if ( isSet( "upload_store" ) == true )
		return ( ConfigApply::applyAlias( path, loc->getPath(), \
					this->_directives->getUploadStore() ) );
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

int	Server::onNewClient( void )
{

	socket_t			clientFd;
	struct sockaddr_in	info;
	
	clientFd = Sockets::acceptConnection( this->_socketFd, info );
	if ( clientFd < 0 )
		return ( -1 );
	if ( !this->receptionist->newClient( clientFd, this->evs, \
			this->receptionist->getServers(), info, this->receptionist ) )
	{
		Log::Error( "Failed to append Request" );
		close( clientFd );
		return ( -1 );
	}
	return ( 1 );
}

int	Server::onEvent( Event &tevent )
{
	if ( tevent.filter & EVFILT_READ )
		return ( onNewClient() );
}
