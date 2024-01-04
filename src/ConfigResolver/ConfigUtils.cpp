/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 16:29:17 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 17:51:04 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigUtils.hpp>

namespace ConfigUtils
{
	int	comparePathReference( const std::string path, \
								const std::string reference )
	{
		StringVector	pathVector;
		StringVector	referenceVector;

		SUtils::split( pathVector, path, "/" );
		SUtils::split( referenceVector, reference, "/" );
		return ( comparePathReference( pathVector, referenceVector ) );
	}

	int	comparePathReference( const StringVector path, \
								const StringVector reference )
	{
		size_t	size = reference.size();
		size_t	cmp = 0;

		if ( path.size() < size )
			return ( -1 );
		while ( cmp < size )
		{
			if ( path[ cmp ] != reference[ cmp ] )
				break ;
			cmp++;
		}
		return ( cmp );
	}

	std::string	pathJoin( const std::string str1, const std::string str2 )
	{
		std::string	path( str1 );
	
		if ( path[ path.length() - 1 ] != '/' )
			path += "/";
		if ( str2[ 0 ] == '/' )
			path += str2.substr( 1 );
		else
			path += str2;
		//if ( path[ path.length() - 1 ] != '/' )
		//	path += "/";
		return ( path );
	}
}
