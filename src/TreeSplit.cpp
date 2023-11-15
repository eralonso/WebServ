/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:11:41 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 17:44:30 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TreeSplit.hpp>

namespace TreeSplit
{
	void	splitOnce( std::string str, std::string& head, std::string& body )
	{
		size_t	pos;

		head = SUtils::leftTrim( str );
		pos = head.find_first_of( ISSPACE );
		body = head.substr( pos, head.length() );
		head = head.substr( 0, pos );
		body = SUtils::trim( body );
	}

	size_t	checkBracets( std::string src )
	{
		int		level;
		size_t	bracet;

		level = 0;
		do
		{
			bracet = src.find_first_of( "{}" );
			if ( bracet == std::string::npos )
				return ( std::string::npos );
			level += src[ bracet ] == '{' ? 1 : -1;
			src.erase( 0, bracet + 1 );
		} while ( level > 0 );
		return ( bracet );
	}

	bool	get_pair( std::string& head, std::string& body, std::string& src )
	{
		size_t	pos;
		size_t	bracet;

		pos = src.find_first_of( "{;" );
		if ( pos == std::string::npos )
			return ( false );
		if ( src[ pos ] == ';' )
		{
			splitOnce( src.substr( 0, pos ), head, body );
			src.erase( 0, pos + 1 );
			return ( true );
		}
		bracet = checkBracets( src );
		head = SUtils::trim( src.substr( 0, pos ) );
		body = SUtils::trim( src.substr( pos + 1, bracet - pos ) );
		src.erase( 0, bracet + 1 );
		return ( true );
	}
} // namespace TreeSplit
