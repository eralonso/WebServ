/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigApply.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 12:28:39 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 17:46:59 by eralonso         ###   ########.fr       */
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

		SUtils::split( pathVector, path, "/" );
		SUtils::split( routeVector, route, "/" );
		return ( applyAlias( routeVector, pathVector, alias ) );
	}

	const std::string	applyAlias( const StringVector& route, \
												const StringVector& path, \
												const std::string alias )
	{
		int	cmp = 0;

		cmp = ConfigUtils::comparePathReference( path, route );
		return ( ConfigUtils::pathJoin( alias, \
					STLUtils::vectorToString< const StringVector >( \
					path.begin() + cmp, path.end(), "/" ) ) );
	}

	const std::string	applyRoot( const std::string& path, \
												const std::string root )
	{
		return ( ConfigUtils::pathJoin( path, root ) );
	}

	const std::string	applyUploadStore( const std::string& path, \
												const std::string uploadRoot )
	{
		return ( ConfigUtils::pathJoin( path, uploadRoot ) );
	}
}
