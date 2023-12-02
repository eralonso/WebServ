/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationInit.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:20:23 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 15:41:35 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Location.hpp>

std::string	Location::_allowedDirectivesAux[ LOCATION_SIZE_DIRECTIVES + 1 ] = 
{
	"root",
	"error_page",
	"client_max_body_size",
	"index",
	"autoindex",
	"alias",
	"return",
	"allow_methods",
	"cgi"
};

ConstStringVector	Location::allowedDirectives( _allowedDirectivesAux, \
											_allowedDirectivesAux \
											+ LOCATION_SIZE_DIRECTIVES );
