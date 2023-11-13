/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:41:54 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 13:18:43 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerConfig.hpp>

ServerConfig::ServerConfig( void )
{
	clientMaxBodySize = 0;
}

ServerConfig::ServerConfig( std::string server, std::string options )
{
	//TODO get attributes from get and
	//process the body to obtain more attributes
	std::string content;
	std::string head;
	std::string body;
	std::array< std::string, SIZE_SERVER_OPTIONS >::iterator	it;
	std::array< std::string, SIZE_SERVER_OPTIONS >	availablesOptions = { \
										"root", "location", \
										"listen", "server_name", \
										"error_page", "client_max_body_size" };
	std::array< t_parse, SIZE_SERVER_OPTIONS >	parseOption = { \
					&ServerConfig::parseRoot, &ServerConfig::parseLocation, \
					&ServerConfig::parseListen, &ServerConfig::parseServerName, \
					&ServerConfig::parseErrorPage, &ServerConfig::parseClientMaxBodySize };

	content = options;
	if (server != "server")
		throw std::logic_error( "Invalid option" );
	while (content.length() > 0)
	{
		if (TreeSplit::get_pair(head, body, content))
		{
			it = std::find( availablesOptions.begin(), availablesOptions.end(), head );
			if ( it == availablesOptions.end() )
				throw std::logic_error( "Unavailable server option" );
			try
			{
				( this->*parseOption[ it - availablesOptions.begin() ] )( body );
			}
			catch ( std::exception& e )
			{
				Log::Error( e.what() );
			}
			//TODO Analyze Head n Body to look for attributes and
			//set them or add them to the containers
			// ports;
			// locations;
			// rootDir;
			try
			{
				Location lc;
				locations.push_back(lc);
			}
			catch(const std::exception& e)
			{
				;
				//TODO LogError and thow exception to inform parent to clean
			}
		}
		head.clear();
		body.clear();
	}
}

ServerConfig::~ServerConfig( void )
{
}

ServerConfig::ServerConfig( const ServerConfig& b )
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
}

ServerConfig&	ServerConfig::operator=( const ServerConfig& b )
{
	ports = b.ports;
	locations = b.locations;
	rootDir = b.rootDir;
	return ( *this );
}

void	ServerConfig::parseRoot( std::string body )
{
	( void )body;
}

void	ServerConfig::parseLocation( std::string body )
{
	( void )body;
}

void	ServerConfig::parseListen( std::string body )
{
	( void )body;
}

void	ServerConfig::parseServerName( std::string body )
{
	( void )body;
}

void	ServerConfig::parseErrorPage( std::string body )
{
	( void )body;
}

void	ServerConfig::parseClientMaxBodySize( std::string body )
{
	( void )body;
}
