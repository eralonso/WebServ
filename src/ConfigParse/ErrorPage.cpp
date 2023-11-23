/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 18:02:40 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/23 18:07:25 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ServerParser.hpp>

void	ServerParser::fillErrorPages( StringVector args )
{
	std::string		uri;
	int				code;

	uri = args.back();
	args.pop_back();
	for ( StringVector::iterator it = args.begin(); it != args.end(); it++ )
	{
		code = parseErrorCode( *it );
		if ( code < 0 )
			throw std::logic_error( INVALID_VALUE_DIRECTIVE( \
						std::string( "error_page" ), *it ) );
		else if ( code == 0 )
			throw std::logic_error( INVALID_RANGE_DIRECTIVE( *it, \
					SUtils::longToString( MIN_ERROR_CODE ), \
					SUtils::longToString( MAX_ERROR_CODE ) ) );
		this->_errorPages[ code ] = uri;
	}
}

int	ServerParser::parseErrorCode( std::string code )
{
	if ( SUtils::isNum( code ) == false )
		return ( -1 );
	if ( SUtils::compareNumbersAsStrings( code, \
				SUtils::longToString( MIN_ERROR_CODE ) ) < 0 \
			|| SUtils::compareNumbersAsStrings( code, \
				SUtils::longToString( MAX_ERROR_CODE ) ) > 0 )
		return ( 0 );
	return ( std::atol( code.c_str() ) );
}
