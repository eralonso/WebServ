/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigApply.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 12:28:39 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/16 11:31:11 by eralonso         ###   ########.fr       */
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

	const std::string	applyUploadStore( const std::string& path, \
												const std::string uploadRoot )
	{
		return ( uploadRoot + path );
	}
}
