/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:31:42 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/22 18:15:20 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTILS_HPP_
# define _UTILS_HPP_

# include <string>
# include <iostream>
# include <sstream>
# include <vector>
# include <string.h>
# include <stdlib.h>

# include <Defines.hpp>

namespace PUtils
{
	void	printInAscii( const char *str );
}

namespace SUtils
{
	std::string		longToString( long int num );
	int				atoi( std::string str );
	long			atol( std::string str );
	void			memset( void *p, int c, size_t size );
	int				strcmp( const char *s1, const char *s2 );
	bool			isNum( std::string num );
	std::string		trim( const std::string str );
	std::string		leftTrim( const std::string str );
	std::string		rightTrim( const std::string str );
	StringVector&	split( std::vector<std::string>& v, \
						std::string strArr, \
						std::string delimiter );
	StringVector	splitOnce( std::string str, \
						std::string del );
	int				compareNumbersAsStrings( \
						const std::string num1, \
						const std::string num2 );
}

namespace STLUtils
{
	template < typename T >
	std::string	vectorToString( typename T::const_iterator begin, \
							typename T::const_iterator end, \
							std::string join );
	template < typename T >
	std::string	mapToString( typename T::const_iterator begin, \
							typename T::const_iterator end );

	template < typename T >
	bool		cmpVector( std::vector< T > first, std::vector< T > second );

	template < typename T >
	int			stringEasyFind( typename T::iterator begin, typename T::iterator end, \
						std::string toFind );

	template < typename T >
	int			charCount( typename T::iterator begin, typename T::iterator end, \
							char toFind );
}

namespace Log
{
	void	Info( const std::string str );
	void	Error( const std::string str );
	void	Success( const std::string str );
}

namespace Binary
{
	std::string		formatBits( std::string bits );
	std::string		decodeAddress( unsigned int address );
	unsigned int	codeAddress( std::string address );
}

#include "Utils.ipp"

#endif
