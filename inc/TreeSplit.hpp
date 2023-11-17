/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:01:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 19:34:19 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _TREESPLIT_HPP_
# define _TREESPLIT_HPP_

# include <string>

# include <Utils.hpp>
# include <Defines.hpp>

namespace TreeSplit
{
	void	splitOnce( std::string str, std::string& head, std::string& body );
	bool	get_pair(std::string& head, std::string& body, std::string& src);
} // namespace TreeSplit

#endif
