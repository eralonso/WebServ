/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:01:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 13:27:25 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _TREESPLIT_HPP
# define _TREESPLIT_HPP

# include <string>

# include <Utils.hpp>

namespace TreeSplit
{
	bool get_pair(std::string& head, std::string& body, std::string& src);
} // namespace TreeSplit

#endif
