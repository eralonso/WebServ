/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:00:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 16:45:06 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SPLITSTRING_HPP_
# define _SPLITSTRING_HPP_

# include <string>
# include <vector>

# include <Defines.hpp>

namespace SplitString
{
	StringVector	split( const std::string& content, const std::string& sep );
}

#endif
