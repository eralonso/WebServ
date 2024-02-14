/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigApply.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 12:28:39 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/14 11:45:46 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ConfigApply.hpp>

namespace ConfigApply
{
	const std::string	applyAlias( const std::string& path, \
												const std::string& route, \
												const std::string alias )
	{
		StringVector	pathVector;
		StringVector	routeVector;
		int				cmp = 0;

		cmp = ConfigUtils::comparePathReference( path, route );
		if ( cmp == -1 )
			return ( path );
		return ( alias + path.substr( cmp ) );
	}

	const std::string	applyRoot( const std::string& path, \
												const std::string root )
	{
		return ( root + path );
	}

}
