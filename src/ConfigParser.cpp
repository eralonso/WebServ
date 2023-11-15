/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:48:38 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 13:23:50 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

std::vector<std::string>&	split( std::vector<std::string>& v,
	std::string strArr,
	std::string delimiter )
{
	std::string	temp;
    int starting;
    int ending;

	starting = 0;
	ending = strArr.find( delimiter );
    while ( ending != -1 )
	{
		temp = strArr.substr( starting, ending-starting );
		if ( temp.length() > 0 )
        	v.push_back( temp );
        starting = ending + delimiter.size();
        ending = strArr.find( delimiter, starting );
    }
	temp = strArr.substr( starting, ending-starting );
	if ( temp.length() > 0 )
		v.push_back( temp );
	return ( v );
}

ConfigParser::ConfigParser( int argc, char **argv )
{
	std::ifstream	myfile;
	std::string		line;
	std::string		content;
	std::string		head;
	std::string		body;

	if ( argc > 2 )
	{
		Log::Error( "Usage: " + std::string( argv[0] ) + " <config_file>" );
		return ;
	}
	myfile.open( argv[1], std::ios_base::in );
	if ( !myfile.is_open() )
	{
		Log::Error( "Invalid file: " + std::string( argv[1] ) );
		return ;
	}
	while ( std::getline ( myfile, line ) )
		content += line + "\n";
	myfile.close();
	while ( content.length() > 0 )
	{
		if ( TreeSplit::get_pair( head, body, content ) )
		{
			try
			{
				ServerConfig	sc( head, body );
				serversConfig.push_back( sc );
			}
			catch ( const std::exception& e )
			{
				Log::Error( e.what() );
				//TODO LogError and clean
				return ;
			}
		}
		else if ( content.length() > 0 )
		{
			Log::Error( "Unexpected end of file, expecting \";\" or \"}\"" );
			return ;
		}
		head.clear();
		body.clear();
	}
}

ConfigParser::~ConfigParser( void )
{
}

std::vector<ServerConfig>& ConfigParser::getServersConfig( void )
{
	return ( serversConfig );
}
