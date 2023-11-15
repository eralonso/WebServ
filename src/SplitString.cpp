/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SplitString.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:05:57 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 15:31:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SplitString.hpp>

namespace SplitString
{
	std::vector<std::string>	split(const std::string& content,
		const std::string& sep)
	{
		std::vector<std::string> ret;
		if (sep.length() < 1)
		{
			ret.push_back(content);
			return (ret);
		}
		size_t content_size = content.length();
		size_t prev_pos = 0;
		size_t pos;
		while (prev_pos < content_size &&
			(pos = content.find(sep, prev_pos)) != std::string::npos)
		{
			size_t chunk_size = pos - prev_pos;
			ret.push_back(content.substr(prev_pos, chunk_size));
			prev_pos = pos + sep.length();
		}
		size_t chunk_size = content_size - prev_pos;
		if (chunk_size > 0)
			ret.push_back(content.substr(prev_pos, chunk_size));
		return ret;
	}
}
