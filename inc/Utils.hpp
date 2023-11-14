/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:31:42 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/14 11:31:07 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <string>
#include <iostream>
#include <sstream>

#include "Defines.hpp"

namespace PUtils
{
	void	printInAscii( const char *str );
}

namespace SUtils
{
	std::string	longToString( long int num );
	std::string	trim( std::string str );
	std::string	leftTrim( std::string str );
	std::string	rightTrim( std::string str );
}

namespace Log
{
	void	Info( const std::string str );
	void	Error( const std::string str );
	void	Success( const std::string str );
}

namespace Binary
{
	std::string	formatBits( std::string bits );
	std::string	decodeAddress( uint32_t address );
	uint32_t	codeAddress( std::string address );
}

#endif
