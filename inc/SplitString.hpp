/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:00:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 15:31:39 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SPLITSTRING_HPP_
# define _SPLITSTRING_HPP_
# include <string>
# include <vector>

namespace SplitString
{
	std::vector<std::string>	split(const std::string& content,
		const std::string& sep);
}

#endif
