/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationInit.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:20:23 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 12:48:10 by eralonso         ###   ########.fr       */
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

std::string	Location::_allowedSimpleDirectivesAux[ LOCATION_SIZE_SIMPLE_DIRECTIVES + 1 ] = 
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

ConstStringVector	Location::allowedSimpleDirectives( \
											_allowedSimpleDirectivesAux, \
	   										_allowedSimpleDirectivesAux \
											+ LOCATION_SIZE_SIMPLE_DIRECTIVES );

ConstStringVector	Location::allowedComplexDirectives;
