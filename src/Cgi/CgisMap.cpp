/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgisMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:18:09 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/21 13:16:04 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CgisMap.hpp>
#include <Utils.hpp>

CgisMap::CgisMap( void ) {}

CgisMap::~CgisMap( void ) {}

CgisMap::CgisMap( const CgisMap& c ): std::map< std::string, std::string >( c ) {}

CgisMap&	CgisMap::operator=( const CgisMap &c )
{
	if ( this != &c )
		std::map< std::string, std::string >::operator=( c );
	return ( *this );
}

int	CgisMap::appendCgi( std::string ext, std::string binary )
{
	this->insert( std::pair< std::string, std::string >( ext, binary ) );
	return ( 1 );
}

std::string	CgisMap::getBinary( std::string ext ) const
{
	Log::Success("CgisMap::getBinary map.size: " + SUtils::longToString(size()));
	try
	{
		return (this->at( ext ));
	}
	catch(const std::exception& e)
	{
		return ( std::string("") );
	}
}

bool	CgisMap::findCgi( std::string ext, std::string& binary )
{
	std::map< std::string, std::string >::iterator	it = this->find( ext );

	if ( it != this->end() )
	{
		binary = it->second;
		return ( true );
	}
	return ( false );
}
