/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 10:19:37 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 10:20:53 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _HEADERS_HPP_
# define _HEADERS_HPP_

# include <vector>
//# include <algorithm>

# include "Header.hpp"

class Headers: public std::vector< Header >
{
private:
public:
	Headers( void );
	Headers( const Headers& b );
	~Headers( void );
	Headers&			operator=( const Headers& b );
	Headers				filterKey( const std::string& key );
	Header				*firstWithKey( const std::string& key );
	const Header		*firstWithKey( const std::string& key ) const;
	void				append( Header& b );
	void				append( const std::string& key, const std::string& value );
	void				replace( Header& b );
	void				replace( const std::string& key, const std::string& value );
	void				erase( const std::string& key );
	void				erase( Header& h );
	std::string			toString( void ) const;
    std::string			getCookies(void) const;
};

#endif
