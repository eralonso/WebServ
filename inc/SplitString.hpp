/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:00:51 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/16 12:01:12 by codespace        ###   ########.fr       */
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
	StringVector	splitHeaderBody(std::string& body, const std::string& content);
}

#endif
