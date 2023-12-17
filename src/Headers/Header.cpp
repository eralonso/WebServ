/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:55:02 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 17:03:23 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header( std::string key, std::string value ): key( key ), \
														value( value ) {}

Header::Header( const Header& b ): key( b.key ), value( b.value ) {}

Header::~Header( void ) {}

Header&	Header::operator=( const Header& b )
{
	this->key = b.key;
	this->value = b.value;
	return ( *this );
}

std::string	Header::getKey( void ) const
{
	return ( key );
}

std::string	Header::getValue( void ) const
{
	return ( value );
}

std::string Header::toString( void ) const
{
	return ( key + ": " + value + HEADER_SEP );
}
