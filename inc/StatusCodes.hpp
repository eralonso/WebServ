/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 18:41:19 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 19:29:16 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _STATUSCODES_HPP_
# define _STATUSCODES_HPP_
# define CODES_NB 69
# include <string>

class StatusCodes
{
private:
	StatusCodes();
	StatusCodes(const StatusCodes&);
	~StatusCodes();
	StatusCodes operator+(const StatusCodes&);
public:
	typedef struct s_dict {unsigned int code; const char *desc;}	t_dict;
	static const t_dict dict[CODES_NB];
	static const std::string decode(unsigned int code);
};

#endif
