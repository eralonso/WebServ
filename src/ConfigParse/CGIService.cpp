/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIService.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:15:53 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 18:59:29 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CGIService.hpp>

CGIService::CGIService( void ): _port( 8000 ) {}

CGIService::CGIService( std::string name, unsigned int port, std::string url,
	std::string extension, std::string interpreter ): _name( name ), \
												_port( port ), \
												_url( url ), \
												_extension( extension ), \
												_interpreter( interpreter ) {}

CGIService::~CGIService( void ) {}

CGIService::CGIService( const CGIService& b ): _name( b._name ), \
												_port( b._port ), \
												_url( b._url ), \
												_extension( b._extension ), \
												_interpreter( b._interpreter ) {}

CGIService& CGIService::operator=( const CGIService& b )
{
	if ( this != &b )
	{
		this->_name = b._name;
		this->_port = b._port;
		this->_url = b._url;
		this->_extension = b._extension;
		this->_interpreter = b._interpreter;
	}
	return ( *this );
}

void	CGIService::setExtension( const std::string extension )
{
	this->_extension = extension;
}

void	CGIService::setInterpreter( const std::string interpreter )
{
	this->_interpreter = interpreter;
}

unsigned int	CGIService::getPort( void ) const
{
	return ( this->_port );
}

std::string		CGIService::getName( void ) const
{
	return ( this->_name );
}

std::string		CGIService::getUrl( void ) const
{
	return ( this->_url );
}

std::string		CGIService::getExtension( void ) const
{
	return ( this->_extension );
}

std::string		CGIService::getInterpreter( void ) const
{
	return ( this->_interpreter );
}

std::ostream&	operator<<( std::ostream& out, const CGIService& cgi )
{
	out << cgi.getInterpreter();
	return ( out );
}

std::ostream&	operator<<( std::ostream& out, const CGIService* cgi )
{
	if ( cgi )
		out << cgi->getInterpreter();
	return ( out );
}
