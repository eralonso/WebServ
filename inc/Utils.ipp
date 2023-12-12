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
	//container to string
	template < typename T >
	std::string	vectorToString( typename T::const_iterator begin, \
							typename T::const_iterator end )
	{
		std::stringstream	str;

		for ( typename T::const_iterator it = begin; it != end; it++ )
		{
			str << *it;
			if ( it + 1 != end )
				str << " ";
		}
		return ( str.str() );
	}

	template < typename T >
	std::string	mapToString( typename T::const_iterator begin, \
							typename T::const_iterator end )
	{
		std::stringstream	str;

		for ( typename T::const_iterator it = begin; it != end; ++it )
		{
			if ( it != begin )
				str << ", ";
			str << it->first << " -> " << it->second;
		}
		return ( str.str() );
	}

	//container compare
	template < typename T >
	bool	cmpVector( std::vector< T > first, std::vector< T > second )
	{
		if ( first.size() != second.size() )
			return ( false );
		for ( size_t i = 0; i < first.size(); i++ )
		{
			if ( first[ i ] != second[ i ] )
				return ( false );
		}
		return ( true );
	}

	//find element in range of iterators
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
