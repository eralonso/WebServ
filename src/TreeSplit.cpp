/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeSplit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:11:41 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 17:46:20 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TreeSplit.hpp>

namespace TreeSplit
{
	bool get_pair(std::string& head, std::string& body, std::string& src)
	{
		int						level = 0;
		size_t					pos;
		size_t					aux;

		pos = src.find_first_of("{;");
		if (pos == std::string::npos)
			return (false);
		if (src[pos] == ';')
		{
			head = src.substr(0, pos);
			body = "";
			src.erase(0, pos + 1);
			return (true);
		}
		level++;
		while(level > 0)
		{
			aux = src.find_first_of("{}");
			if (aux == std::string::npos)
				return (false);
			level += src[aux] == '{' ? 1 : -1;
		}
		head = src.substr(0, pos);
		body = src.substr(pos + 1, aux - pos);
		src.erase(0, aux + 1);
		return (true);
	}
} // namespace TreeSplit
