/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:59:08 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/29 17:00:55 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <Directives.hpp>

long	Directives::getMeasureLimit( int unit )
{
	return ( std::numeric_limits< long >::max() >> ( 10 * unit ) );
}

int	Directives::parseMeasure( std::string number )
{
	std::string				units = "bkmg";
	std::string::iterator	it;
	size_t					i;
	char					unit;
	int						res;

	res = 0;
	i = number.find_first_not_of( "0123456789" );
	if ( i != std::string::npos )
	{
		unit = std::tolower( number[ i ] );
		it = std::find( units.begin(), units.end(), unit );
		if ( it == units.end() || number[ i + 1 ] != '\0' )
			return ( -1 );
		res = it - units.begin();
	}
	return ( res );
}

long	Directives::parseSize( std::string number )
{
	int		measure;

	measure = parseMeasure( number );
	if ( measure == -1 )
		throw std::logic_error( "Invalid measurement unit" );
	if ( number.find_first_not_of( "0123456789" ) != std::string::npos )
		number.erase( number.length() - 1, 1 );
	if ( SUtils::compareNumbersAsStrings( number, \
				SUtils::longToString( getMeasureLimit( measure ) ) ) > 0 )
		throw std::logic_error( "Number too large" );
	return ( std::atol( number.c_str() ) << ( 10 * measure ) );
}
