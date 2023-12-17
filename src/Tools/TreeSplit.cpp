/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:11:41 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 16:47:08 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TreeSplit.hpp>

namespace TreeSplit
{
	void	splitOnceBySpace( const std::string str, std::string& head, \
								std::string& body )
	{
		size_t		pos;
		std::string	aux;

		body = "";
		aux = SUtils::trim( str );
		pos = aux.find_first_of( ISSPACE );
		if ( pos != std::string::npos )
		{
			head = aux.substr( 0, pos );
			body = aux.substr( pos, std::string::npos );
			body = SUtils::leftTrim( body );
		}
		else
			head = aux;
	}

	size_t	checkBracets( std::string src )
	{
		int			level;
		size_t		bracet;
		size_t		ret;
		std::string	copy;

		level = 0;
		ret = 0;
		copy = src;
		do
		{
			bracet = src.find_first_of( "{}" );
			if ( bracet == std::string::npos \
					|| ( src[ bracet ] == '}' && level == 0 ) )
				return ( std::string::npos );
			level += src[ bracet ] == '{' ? 1 : -1;
			src.erase( 0, bracet + 1 );
			ret += bracet + 1;
		} while ( level > 0 );
		ret--;
		return ( ret );
	}

	int	get_pair( std::string& head, std::string& body, std::string& src )
	{
		size_t	pos;
		size_t	bracet;

		pos = src.find_first_of( "{;" );
		if ( pos == std::string::npos )
			return ( NOT_A_SEPARATOR );
		if ( src[ pos ] == ';' )
		{
			splitOnceBySpace( src.substr( 0, pos ), head, body );
			src.erase( 0, pos + 1 );
			src = SUtils::leftTrim( src );
			return ( SEMICOLON_SEPARATOR );
		}
		bracet = checkBracets( src );
		if ( bracet == std::string::npos )
			throw std::logic_error( "Expected \"}\"" );
		head = SUtils::trim( src.substr( 0, pos ) );
		body = SUtils::trim( src.substr( pos + 1, bracet - pos - 1 ) );
		src.erase( 0, bracet + 1 );
		src = SUtils::leftTrim( src );
		return ( BRACET_SEPARATOR );
	}
} // namespace TreeSplit
