/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIService.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:14:43 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 18:45:02 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGISERVICE_HPP_
# define _CGISERVICE_HPP_

# include <string>
# include <iostream>

# include <TypesDefines.hpp>

class CGIService
{
private:
	std::string		_name;
	unsigned int	_port;
	std::string		_url;
	std::string		_extension;
	std::string		_interpreter;
public:
	CGIService( void );
	CGIService( std::string name, unsigned int port, std::string url, \
						std::string extension, std::string interpreter );
	~CGIService( void );
	CGIService( const CGIService& b );
	CGIService& 	operator=( const CGIService& b );
public:
	void			setExtension( const std::string extension );
	void			setInterpreter( const std::string interpreter );
	std::string		getName( void ) const;
	unsigned int	getPort( void ) const;
	std::string		getUrl( void ) const;
	std::string		getExtension( void ) const;
	std::string		getInterpreter( void ) const;
};

std::ostream&	operator<<( std::ostream& out, const CGIService& cgi );
std::ostream&	operator<<( std::ostream& out, const CGIService* cgi );

#endif
