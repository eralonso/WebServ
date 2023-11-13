/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:11:41 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 19:42:03 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TreeSplit.hpp>

namespace TreeSplit
{
	bool get_pair( std::string& head, std::string& body, std::string& src )
	{
		int		level;
		size_t	pos;
		size_t	bracket;
		size_t	aux;

		pos = src.find_first_of( "{;" );
		if ( pos == std::string::npos )
			return ( false );
		if ( src[ pos ] == ';' )
		{
			aux = src.substr( 0, pos ).find_first_of( "\t " ) != std::string::npos ? \
				  src.substr( 0, pos ).find_first_of( "\t " ) : pos;
			head = src.substr( 0, aux );
			body = aux == pos ? "" : src.substr( aux, pos);
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
		head = src.substr( 0, pos );
		body = src.substr( pos + 1, bracket - pos );
		src.erase( 0, bracket + 1 );
		return ( true );
	}
} // namespace TreeSplit
