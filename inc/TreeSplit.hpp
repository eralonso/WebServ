/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:01:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 14:32:01 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _TREESPLIT_HPP_
# define _TREESPLIT_HPP_

# include <string>

# include <Utils.hpp>
# include <Defines.hpp>

namespace TreeSplit
{
	void	splitOnceBySpace( std::string str, std::string& head, std::string& body );
	int		get_pair(std::string& head, std::string& body, std::string& src);
} // namespace TreeSplit

#endif
