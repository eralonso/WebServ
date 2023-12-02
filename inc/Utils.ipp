/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.ipp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:31:42 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/18 17:49:03 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTILS_IPP_
# define _UTILS_IPP_

# include "Utils.hpp"

namespace STLUtils
{
	template < typename T >
	std::string	vectorToString( typename T::iterator begin, typename T::iterator end )
	{
		std::stringstream	str;

		for ( typename T::iterator it = begin; it != end; it++ )
		{
			str << *it;
			if ( it + 1 != end )
				str << " ";
		}
		return ( str.str() );
	}

	template < typename T >
	std::string	mapToString( typename T::iterator begin, typename T::iterator end )
	{
		std::stringstream	str;

		for ( typename T::iterator it = begin; it != end; ++it )
		{
			if ( it != begin )
				str << ", ";
			str << it->first << " -> " << it->second;
		}
		return ( str.str() );
	}

	template < typename T >
	int	stringEasyFind( typename T::iterator begin, typename T::iterator end, \
						std::string toFind )
	{
		typename T::iterator	it;

		it = std::find( begin, end, toFind );
		if ( it == end )
			return ( -1 );
		return ( it - begin );
	}
}

#endif
