/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:05:57 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 16:44:51 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SplitString.hpp>

namespace SplitString
{
	StringVector	split( const std::string& content, const std::string& sep )
	{
		StringVector	ret;
		size_t			content_size = content.length();
		size_t			prev_pos = 0;
		size_t			pos;
		size_t			chunk_size;

		if ( sep.length() < 1 )
		{
			ret.push_back( content );
			return ( ret );
		}
		while ( prev_pos < content_size \
				&& ( pos = content.find( sep, prev_pos ) ) != std::string::npos )
		{
			chunk_size = pos - prev_pos;
			if ( chunk_size > 0 )
				ret.push_back( content.substr( prev_pos, chunk_size ) );
			prev_pos = pos + sep.length();
		}
		chunk_size = content_size - prev_pos;
		if ( chunk_size > 0 )
			ret.push_back( content.substr( prev_pos, chunk_size ) );
		return ( ret );
	}
}
