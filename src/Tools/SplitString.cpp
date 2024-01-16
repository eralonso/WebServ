/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:05:57 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/16 16:27:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SplitString.hpp>
#include <Utils.hpp>

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

	StringVector	splitHeaderBody(std::string& body, const std::string& content)
	{
		StringVector	ret;
		size_t			content_size = content.length();
		size_t			prev_pos = 0;
		size_t			pos;
		size_t			chunk_size;
		bool			emptyLine = false;

		while ( prev_pos < content_size && !emptyLine \
				&& ( pos = content.find( "\n", prev_pos ) ) != std::string::npos )
		{
			chunk_size = pos - prev_pos;
			emptyLine = chunk_size == 0 || (chunk_size == 1 && content[pos] <= ' ');
			if (! emptyLine)
			{
				std::string line = content.substr( prev_pos, chunk_size );
				ret.push_back( line );
			}
			prev_pos = pos + 1;
		}
		chunk_size = content_size - prev_pos;
		if ( chunk_size > 0 )
			body = content.substr( prev_pos, chunk_size );
		else
			body = std::string("");
		return ( ret );
	}

	bool		splitHeaderLine(std::string& key, std::string& value, const std::string& line)
	{
		size_t pos = line.find( ":", 0 );
		
		if ( pos != std::string::npos)
		{
			key = SUtils::trim(line.substr( 0, pos ));
			value = SUtils::trim(line.substr( pos + 1, line.size() - pos));
		}
		return (true);
	}
}
