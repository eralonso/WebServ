/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:55:51 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/02 11:47:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Directives.hpp>

Directives::Directives( void ): _port( 8000 ), \
								_host( "0.0.0.0" ), \
								_clientMaxBodySize( 1 << 20 ), \
								_locations( Location::locationCompare ), \
								_isEmpty( true )
{
	this->_return.first = -1;
	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		this->_isSet[ DirectivesParser::directivesList[ i ] ] = false;
}

Directives::~Directives( void )
{
	deleteLocationsSet();
}

Directives::Directives( const Directives& d ):
								_root( d._root ), \
								_port( d._port ), \
								_host( d._host ), \
								_serverNames( d._serverNames ), \
								_errorPages( d._errorPages ), \
								_clientMaxBodySize( d._clientMaxBodySize ), \
								_uploadStore( d._uploadStore ), \
								_index( d._index ), \
								_autoindex( d._autoindex ), \
								_alias( d._alias ), \
								_return( d._return ), \
								_allowMethods( d._allowMethods ), \
								_cgis( d._cgis ), \
								_servers( d._servers ), \
								_locations( Location::locationCompare ), \
								_isSet( d._isSet ),
								_isEmpty( d._isEmpty )
{
	copyLocationsSet( d._locations );
}

Directives&	Directives::operator=( const Directives& d )
{
	if ( this != &d )
	{
		this->_root = d._root;
		this->_port = d._port;
		this->_host = d._host;
		this->_serverNames = d._serverNames;
		this->_errorPages = d._errorPages;
		this->_clientMaxBodySize = d._clientMaxBodySize;
		this->_uploadStore = d._uploadStore;
		this->_index = d._index;
		this->_autoindex = d._autoindex;
		this->_alias = d._alias;
		this->_return = d._return;
		this->_allowMethods = d._allowMethods;
		this->_cgis = d._cgis;
		this->_servers = d._servers;
		this->_isSet = d._isSet;
		this->_isEmpty = d._isEmpty;
		deleteLocationsSet();
		copyLocationsSet( d._locations );
	}
	return ( *this );
}

void	Directives::deleteLocationsSet( void )
{
	for ( LocationsSet::const_iterator it = this->_locations.begin(); \
			it != this->_locations.end(); it++ )
	{
		if ( *it != NULL )
			delete *it;
	}
	this->_locations.clear();
}

void	Directives::copyLocationsSet( const LocationsSet& locations )
{
	for ( LocationsSet::const_iterator it = locations.begin(); \
			it != locations.end(); it++ )
	{
		if ( *it != NULL )
			this->_locations.insert( new Location( **it ) );
	}
}

std::string	Directives::getRoot( void ) const { return ( this->_root ); }

int	Directives::getPort( void ) const { return ( this->_port ); }

std::string	Directives::getHost( void ) const { return ( this->_host ); }

const StringVector&	Directives::getServerNames( void ) const { return ( this->_serverNames ); }

const ErrorPagesMap&	Directives::getErrorPages( void ) const { return ( this->_errorPages ); }

unsigned int	Directives::getClientMaxBodySize( void ) const { return ( this->_clientMaxBodySize ); }

std::string	Directives::getUploadStore( void ) const { return ( this->_uploadStore ); }

const StringVector&	Directives::getIndex( void ) const { return ( this->_index ); }

bool	Directives::getAutoindex( void ) const { return ( this->_autoindex ); }

std::string	Directives::getAlias( void ) const { return ( this->_alias ); }

const ReturnPair&	Directives::getReturn( void ) const { return ( this->_return ); }

const ActionMask&	Directives::getAllowMethods( void ) const { return ( this->_allowMethods ); }

const CgisMap&	Directives::getCgis( void ) const { return ( this->_cgis ); }

const ServersVector&	Directives::getServers( void ) const { return ( this->_servers ); }

const LocationsSet&	Directives::getLocations( void ) const { return ( this->_locations ); }

const StringBoolMap&	Directives::getIsSet( void ) const { return ( this->_isSet ); }

bool	Directives::isEmpty( void ) const { return ( this->_isEmpty ); }

bool	Directives::isSet( const std::string& key ) { return ( this->_isSet[ key ] ); };

void	Directives::print( void ) const
{
	Log::Info( "[ Config ] root: " + this->_root );
	Log::Info( "[ Config ] host: " + this->_host );
	Log::Info( "[ Config ] port: " + SUtils::longToString( this->_port ) );
	Log::Info( "[ Config ] server_name: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_serverNames.begin(), this->_serverNames.end(), " " ) );
	Log::Info( "[ Config ] error_page: " \
				+ STLUtils::mapToString< ErrorPagesMap >( \
				this->_errorPages.begin(), this->_errorPages.end() ) );
	Log::Info( "[ Config ] client_max_body_size: " \
				+ SUtils::longToString( this->_clientMaxBodySize ) );
	Log::Info( "[ Config ] upload_store: " + this->_uploadStore );
	Log::Info( "[ Config ] index: " \
				+ STLUtils::vectorToString< StringVector >( \
				this->_index.begin(), this->_index.end(), " " ) );
	Log::Info( "[ Config ] autoindex: " + std::string( \
				this->_autoindex == true ? "on" : "off" ) );
	Log::Info( "[ Config ] alias: " + this->_alias );
	Log::Info( "[ Config ] return: " \
				+ SUtils::longToString( this->_return.first ) \
				+ " -> " \
				+ this->_return.second );
	Log::Info( "[ Config ] allow_methods: " \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::GET ) == true ? "GET " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::POST ) == true ? "POST " : "" ) \
				+ std::string( this->_allowMethods.getAction( \
						ActionMask::DELETE ) == true ? "DELETE" : "" ) );
	// Log::Info( "[ Config ] cgi: "
	// 			+ STLUtils::mapToString< std::map< std::string, std::string > >(
	// 			this->_cgis.begin(), this->_cgis.end() ) );
	Log::Info( "[ Config ] servers: " );
	Log::Info( "[ Config ] location: " );
	std::cout << std::endl;
}
