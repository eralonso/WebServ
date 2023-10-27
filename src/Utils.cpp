/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:34:13 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/27 11:30:45 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace PUtils
{
	void	printInAscii( const char *str )
	{
		for ( uint32_t i = 0; str[ i ] != '\0'; i++ )
		{
			std::cout << static_cast< int >( str[ i ] );
			if ( str[ i ] != '\n' && str[ i + 1 ] != '\0' )
				std::cout << " ";
			else if ( str[ i + 1 ] != '\0' )
				std::cout << "\n";
		}
		std::cout << std::endl;
	}
}

namespace SUtils
{
	std::string&	LongToString( long int num )
	{
		std::stringstream	ss;
	
		ss << num;
		return ( ss.str() );
	}
}
