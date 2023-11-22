/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 10:20:03 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 15:00:15 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Headers.hpp"
#include <algorithm>

Headers::Headers(){}

Headers::Headers(const Headers& b) : std::vector<Header>(b){}

Headers::~Headers(){}

Headers&	Headers::operator=(const Headers& b)
{
	std::vector<Header>::operator=(b);
	return (*this);
}

Headers		Headers::filterKey(const std::string& key)
{
	Headers fil;
	Headers::iterator it = begin();
	Headers::iterator ite = end();
	while (it != ite)
	{
		if (it->getKey() == key)
			fil.push_back(*it);
		it++;
	}
	return (fil);
}

Header			*Headers::firstWithKey(const std::string& key)
{
	Headers::iterator it = begin();
	Headers::iterator ite = end();
	while (it != ite)
	{
		if (it->getKey() == key)
			return (&(*it));
		it++;
	}
	return (nullptr);
}

void			Headers::append(Header& b)
{
	push_back(b);
}

void			Headers::append(const std::string& key, const std::string& value)
{
	Header b(key, value);
	push_back(b);
}

void			Headers::replace(Header& b)
{
	replace (b.getKey(), b.getValue());
	push_back(b);
}

void			Headers::replace(const std::string& key, const std::string& value)
{
	Header b(key, value);
	Header *p = firstWithKey(key);
	if (p)
		*p = b;
	else
		push_back(b);
}

void			Headers::erase(const std::string& key)
{
	Headers::iterator it = begin();
	Headers::iterator ite = end();
	while (it != ite)
	{
		if (it->getKey() == key)
			std::vector<Header>::erase(it);
		else
			it++;
	}
}

void			Headers::erase(Header& h)
{
	erase(h.getKey());
}

std::string		Headers::toString(void)
{
	std::string	ret;
	Headers::iterator it = begin();
	Headers::iterator ite = end();
	while (it != ite)
	{
		ret += it->toString();
		it++;
	}
	return std::string(ret);
}
