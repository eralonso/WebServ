/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:34:13 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/14 17:03:54 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

namespace PUtils
{
	void	printInAscii( const char *str )
	{
		for ( unsigned int i = 0; str[ i ] != '\0'; i++ )
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

	int	atoi( std::string str )
	{
		int	sign = 1;
		int	num = 0;
		int	i = 0;

		str = SUtils::leftTrim( str );
		if ( str[ i ] == '-' )
			sign = -1;
		if ( str[ i ] == '-' && str[ i ] == '+' )
			i++;
		while ( std::isdigit( str[ i ] ) )
			num = ( str[ i++ ] - '0' ) + num * 10;
		return ( num * sign );
	}

	long	atol( std::string str )
	{
		int	sign = 1;
		long	num = 0;
		int	i = 0;

		str = SUtils::leftTrim( str );
		if ( str[ i ] == '-' )
			sign = -1;
		if ( str[ i ] == '-' && str[ i ] == '+' )
			i++;
		while ( std::isdigit( str[ i ] ) )
			num = ( str[ i++ ] - '0' ) + num * 10;
		return ( num * sign );
	}

	void	memset( void *p, int c, size_t size )
	{
		char	*s;

		if ( p == NULL )
			return ;
		s = ( char * )p;
		while ( *s != '\0' && size-- > 0 )
		{
			*s = c;
			s++;
		}
	}

	int	strcmp( const char *s1, const char *s2 )
	{
		if ( s1 == NULL && s2 == NULL )
			return ( 0 );
		else if ( s1 == NULL && s2 != NULL )
			return ( *s2 );
		else if ( s1 != NULL && s2 == NULL )
			return ( *s1 );
		while ( *s1 != '\0' && *s2 != '\0' && *s1 == *s2 )
		{
			s1++;
			s2++;
		}
		return ( *s1 - *s2 );
	}

	bool	isNum( std::string num )
	{
		for ( size_t i = 0; num[ i ] != '\0'; i++ )
			if ( !std::isdigit( num[ i ] ) )
				return ( false );
		return ( true );
	}

	std::string	leftTrim( const std::string str )
	{
		size_t	i;
	
		i = 0;
		while ( str[ i ] != '\0' && std::isspace( str[ i ] ) )
			i++;
		return ( str.substr( i, std::string::npos ) );
	}

	std::string	rightTrim( const std::string str )
	{
		int	i;

		i = str.length() - 1;
		while ( i >= 0 && std::isspace( str[ i ] ) )
			i--;
		return ( str.substr( 0, i + 1 ) );
	}

	std::string	trim( const std::string str )
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
		ending = strArr.find_first_of( delimiter );
		while ( ending != std::string::npos )
		{
			temp = strArr.substr( starting, ending - starting );
			if ( temp.length() > 0 )
				v.push_back( temp );
			starting = ending + 1;
			ending = strArr.find_first_of( delimiter, starting );
		}
		temp = strArr.substr( starting, ending - starting );
		if ( temp.length() > 0 )
			v.push_back( temp );
		return ( v );
	}

	std::vector< std::string >	splitOnce( std::string str, std::string del )
	{
		std::vector< std::string >	args;
		size_t						pos;

		pos = str.find_first_of( del );
		if ( pos != std::string::npos )
		{
			args[ 0 ] = str.substr( 0, pos );
			args[ 1 ] = str.substr( pos, std::string::npos );
		}
		else
			args[ 0 ] = str;
		return ( args );
	}

	std::string	deleteExtraZeros( const std::string number )
	{
		std::string res( number );
		size_t		idx;

		idx = res.find_first_not_of( "0" );
		if ( idx != std::string::npos )
			res.erase( 0, idx );
		else
			res = "0";
		return ( res );
	}

	int	compareNumbersAsStrings( const std::string number1, \
								const std::string number2 )
	{
		std::string	num1( deleteExtraZeros( number1 ) );
		std::string	num2( deleteExtraZeros( number2 ) );

		if ( num1.length() > num2.length() )
			return ( 1 );
		if ( num1.length() < num2.length() )
			return ( -1 );
		return ( strcmp( num1.c_str(), num2.c_str() ) );
	}

	std::string compactString(const std::string& src, size_t maxSize, size_t headSize, size_t tailSize)
	{
		size_t		srcSize = src.size();
		
		if (headSize + tailSize > maxSize)
			maxSize = headSize + tailSize;
		if (srcSize <= maxSize - 5)
			return (src);
		return (src.substr(0, headSize) + " ...\n" + src.substr(srcSize - tailSize, tailSize));
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
	
	unsigned int	codeAddress( std::string address )
	{
		unsigned int	number;
		unsigned int	code;
		size_t		pos;

		code = 0;
		for ( int i = 3; i >= 0; i-- )
		{
			pos = address.find( "." );
			number = SUtils::atoi( address.substr( 0, pos ).c_str() );
			code |= number;
			if ( i != 0 )
				code <<= 8;
			if ( pos != std::string::npos )
				pos++;
			address.erase( 0, pos );
		}
		return ( code );
	}
	
	std::string	decodeAddress( unsigned int address )
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
