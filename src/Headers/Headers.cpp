/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 10:20:03 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 17:04:00 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"

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
		if ( toLower( it->getKey() ) == toLower( key ) )
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
		if ( toLower( it->getKey() ) == toLower( key ) )
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

std::string	Headers::toString( void )
{
	std::string			ret;
	Headers::iterator	it = this->begin();
	Headers::iterator	ite = this->end();

	while ( it != ite )
	{
		ret += it->toString();
		it++;
	}
	return ( ret );
}
std::string	Headers::toLower( std::string src )
{
	for ( size_t i = 0; src[ i ] != '\0'; i++ )
	{
		if ( src[ i ] >= 'A' && src[ i ] <= 'Z' )
			src[ i ] += ( 'a' - 'A' );
	}
	return ( src );
}
