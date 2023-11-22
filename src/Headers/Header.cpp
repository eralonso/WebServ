/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:55:02 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 14:58:31 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Header.hpp"

Header::Header(std::string key, std::string value) :
	key(key), value(value){}

Header::Header(const Header& b) : key(b.key), value(b.value){}

Header::~Header(){}

Header&	Header::operator=(const Header& b)
{
	key = b.key;
	value = b.value;
	return (*this);
}

std::string			Header::getKey()
{
	return key;
}

std::string 		Header::getValue()
{
	return value;
}
std::string			Header::toString()
{
	return key + ": " + value + HEADER_SEP;
}
