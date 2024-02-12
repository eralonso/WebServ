/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 10:20:03 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/12 17:51:59 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"
#include "Utils.hpp"

Headers::Headers( void ) {}

Headers::Headers( const Headers& b ): std::vector< Header >( b ) {}

Headers::~Headers( void ) {}

Headers&	Headers::operator=( const Headers& b )
{
	if ( this != &b )
		std::vector< Header >::operator=( b );
	return ( *this );
}

Headers	Headers::filterKey( const std::string& key )
{
	Headers				fil;
	Headers::iterator	it = this->begin();
	Headers::iterator	ite = this->end();

	while ( it != ite )
	{
		if ( SUtils::toLower( it->getKey() ) == SUtils::toLower( key ) )
			fil.push_back( *it );
		it++;
	}
	return ( fil );
}

Header	*Headers::firstWithKey( const std::string& key )
{
	Headers::iterator	it = this->begin();
	Headers::iterator	ite = this->end();

	while ( it != ite )
	{
		if ( SUtils::toLower( it->getKey() ) == SUtils::toLower( key ) )
			return ( &( *it ) );
		it++;
	}
	return ( NULL );
}

const Header	*Headers::firstWithKey( const std::string& key ) const
{
	Headers::const_iterator	it = this->begin();
	Headers::const_iterator	ite = this->end();

	while ( it != ite )
	{
		if ( SUtils::toLower( it->getKey() ) == SUtils::toLower( key ) )
			return ( &( *it ) );
		it++;
	}
	return ( NULL );
}

void	Headers::append( Header& b )
{
	this->push_back( b );
}

void	Headers::append( const std::string& key, const std::string& value )
{
	Header	b( key, value );

	this->push_back( b );
}

void	Headers::replace( Header& b )
{
	replace( b.getKey(), b.getValue() );
}

void	Headers::replace( const std::string& key, const std::string& value )
{
	Header	b( key, value );
	Header	*p = firstWithKey( key );

	if ( p != NULL )
		*p = b;
	else
		this->push_back( b );
}

void	Headers::erase( const std::string& key )
{
	Headers::iterator	it = this->begin();
	Headers::iterator	ite = this->end();

	while ( it != ite )
	{
		if ( it->getKey() == key )
			std::vector< Header >::erase( it );
		else
			it++;
	}
}

void	Headers::erase( Header& h )
{
	erase( h.getKey() );
}

std::string	Headers::toString( void ) const
{
	std::string				ret;
	Headers::const_iterator	it = this->begin();
	Headers::const_iterator	ite = this->end();

	while ( it != ite )
	{
		ret += it->toString();
		it++;
	}
	return ( ret );
}

std::string Headers::getCookies( void ) const
{
	std::string	ret;
	std::vector<Header>::const_iterator it = begin();
	std::vector<Header>::const_iterator ite = end();

	while (it != ite)
	{
		if (SUtils::toLower(it->getKey()) == "cookie")
			ret += it->getValue() + ";";
		it++;
	}
	return (ret);	
}