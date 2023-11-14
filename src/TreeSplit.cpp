/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:11:41 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:11:17 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TreeSplit.hpp>

namespace TreeSplit
{
	void	splitSemicolon( std::string str, std::string& head, std::string& body )
	{
		size_t	pos;

		head = SUtils::leftTrim( str );
		pos = head.find_first_of( ISSPACE );
		body = head.substr( pos, head.length() );
		head = head.substr( 0, pos );
		body = SUtils::trim( body );
	}

	bool	get_pair( std::string& head, std::string& body, std::string& src )
	{
		int		level;
		size_t	pos;
		size_t	bracket;

		pos = src.find_first_of( "{;" );
		if ( pos == std::string::npos )
			return ( false );
		if ( src[ pos ] == ';' )
		{
			splitSemicolon( src.substr( 0, pos ), head, body );
			src.erase( 0, pos + 1 );
			return ( true );
		}
		level = 1;
		while ( level > 0 )
		{
			bracket = src.find_first_of( "{}" );
			if ( bracket == std::string::npos )
				return ( false );
			level += src[ bracket ] == '{' ? 1 : -1;
		}
		head = SUtils::trim( src.substr( 0, pos ) );
		body = SUtils::trim( src.substr( pos + 1, bracket - pos ) );
		src.erase( 0, bracket + 1 );
		return ( true );
	}
} // namespace TreeSplit
