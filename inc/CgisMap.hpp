/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgisMap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 11:18:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/20 18:59:37 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGISMAP_HPP_
# define _CGISMAP_HPP_

# include <map>
# include <string>

class CgisMap: std::map< std::string, std::string >
{
private:
public:
	CgisMap( void );
	~CgisMap( void );
	CgisMap( const CgisMap& b );
	CgisMap&	operator=( const CgisMap& b );
	int			appendCgi( std::string ext, std::string binary );
	std::string	getBinary( std::string ext ) const;
	bool		findCgi( std::string ext, std::string &binary );
};

#endif
