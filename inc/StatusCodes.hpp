/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 18:41:19 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 17:18:37 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _STATUSCODES_HPP_
# define _STATUSCODES_HPP_

# include <string>

# define CODES_NB 69

class StatusCodes
{
private:
	StatusCodes( void );
	StatusCodes( const StatusCodes& );
	~StatusCodes( void );
	StatusCodes	operator=( const StatusCodes& );
public:
	typedef struct s_dict {
		unsigned int code;
		const char *desc;
	}	t_dict;
public:
	static const t_dict	dict[ CODES_NB ];
public:
	static const std::string	decode( unsigned int code );
};

#endif
