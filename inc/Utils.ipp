/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.ipp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:31:42 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/22 18:20:25 by omoreno-         ###   ########.fr       */
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
							typename T::const_iterator end, \
							std::string join )
	{
		std::stringstream	str;

		for ( typename T::const_iterator it = begin; it != end; it++ )
		{
			if ( it != begin )
				str << join;
			str << *it;
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

		it = begin; 
		while ( it != end && ( *it ) != toFind )
			it++;
		//it = std::find( begin, end, toFind );
		if ( it == end )
			return ( -1 );
		return ( it - begin );
	}

	template < typename T >
	int	charCount( typename T::iterator begin, typename T::iterator end, \
					char toFind )
	{
		size_t	amount;

		amount = 0; 
		for ( typename T::iterator it = begin; it != end; it++ )
			if ( *it == toFind )
				amount++;
		return ( amount );
	}
}

#endif
