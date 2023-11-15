/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:34:13 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 18:22:30 by eralonso         ###   ########.fr       */
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

namespace Log
{
	void	Info( const std::string str )
	{
		std::cout << YELLOW << "Info: " << str << DEF << std::endl;
	}
	
	void	Error( const std::string str )
	{
		std::cerr << RED << "Error: " << str << DEF << std::endl;
	}

	void	Success( const std::string str )
	{
		std::cout << GREEN << "Success: "<< str << DEF << std::endl;
	}	
}

namespace SUtils
{
	std::string	longToString( long int num )
	{
		std::stringstream	ss;
	
		ss << num;
		return ( ss.str() );
	}

	bool	isNum( std::string num )
	{
		for ( size_t i = 0; num[ i ] != '\0'; i++ )
			if ( !std::isdigit( num[ i ] ) )
				return ( false );
		return ( true );
	}

	std::string	leftTrim( std::string str )
	{
		size_t	i;
	
		i = 0;
		while ( str[ i ] != '\0' && std::isspace( str[ i ] ) )
			i++;
		return ( str.substr( i, std::string::npos ) );
	}

	std::string	rightTrim( std::string str )
	{
		int	i;

		i = str.length();
		while ( i >= 0 && std::isspace( str[ i ] ) )
			i--;
		i = i < 0 ? 0 : i;
		return ( str.substr( 0, i ) );
	}

	std::string	trim( std::string str )
	{
		return ( rightTrim( leftTrim( str ) ) );
	}

	std::vector<std::string>&	split( std::vector<std::string>& v, \
										std::string strArr, \
										std::string delimiter )
	{
		std::string	temp;
		size_t		starting;
		size_t		ending;

		starting = 0;
		ending = strArr.find( delimiter );
		while ( ending != std::string::npos )
		{
			temp = strArr.substr( starting, ending - starting );
			if ( temp.length() > 0 )
				v.push_back( temp );
			starting = ending + delimiter.size();
			ending = strArr.find( delimiter, starting );
		}
		temp = strArr.substr( starting, ending - starting );
		if ( temp.length() > 0 )
			v.push_back( temp );
		return ( v );
	}

	int	compareNumbersAsStrings( const std::string num1, \
								const std::string num2 )
	{
		if ( num1.length() > num2.length() )
			return ( 1 );
		if ( num1.length() < num2.length() )
			return ( -1 );
		return ( std::strcmp( num1.c_str(), num2.c_str() ) );
	}
}

namespace Binary
{
	std::string	formatBits( std::string bits )
	{
		std::string	formated;

		for ( unsigned int i = 0; bits[ i ] != '\0'; i++ )
		{
			if ( i > 0 && i % 8 == 0 )
				formated += ".";
			formated += bits[ i ];
		}
		return ( formated );
	}
	
	uint32_t	codeAddress( std::string address )
	{
		uint32_t	number;
		uint32_t	code;
		size_t		pos;

		code = 0;
		for ( int i = 3; i >= 0; i-- )
		{
			pos = address.find( "." );
			number = std::atoi( address.substr( 0, pos ).c_str() );
			code |= number;
			if ( i != 0 )
				code <<= 8;
			if ( pos != std::string::npos )
				pos++;
			address.erase( 0, pos );
		}
		return ( code );
	}
	
	std::string	decodeAddress( uint32_t address )
	{
		int			number;
		std::string	decode;

		for ( int i = 3; i >= 0; i-- )
		{
			number = 0;
			for ( int j = 0; j < 8; j++ )
				number |= ( ( address >> ( i * 8 ) ) & ( 1 << j ) );
			decode += SUtils::longToString( number );
			if ( i != 0 )
				decode += ".";
		}
		return ( decode );
	}
}
