/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:54:48 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 17:03:04 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _HEADER_HPP_
# define _HEADER_HPP_

# include <string>

# define HEADER_SEP "\r\n"

class Header
{
private:
	std::string	key;	
	std::string	value;	
public:
	Header( std::string key, std::string value );
	Header( const Header& b );
	~Header( void );
	Header&		operator=( const Header& b );
	std::string	getKey( void ) const;
	std::string	getValue( void ) const;
	std::string	toString( void ) const;
};

#endif
