/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgisMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:18:09 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/14 12:07:24 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CgisMap.hpp>
#include <Utils.hpp>

CgisMap::CgisMap( void ) {}

CgisMap::~CgisMap( void ) {}

CgisMap::CgisMap( const CgisMap& c ): CgiDict( c ) {}

CgisMap&	CgisMap::operator=( const CgisMap &c )
{
	if ( this != &c )
		CgiDict::operator=( c );
	return ( *this );
}

int	CgisMap::appendCgi( std::string ext, std::string binary )
{
	this->insert( CgiDictPair( ext, binary ) );
	return ( 1 );
}

std::string	CgisMap::getBinary( std::string ext ) const
{
	try
	{
		return (this->at( ext ));
	}
	catch(const std::exception& e)
	{
		return ( std::string("") );
	}
}
