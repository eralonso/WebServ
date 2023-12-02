/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:07:44 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/02 12:52:49 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

std::string	Server::_allowedDirectivesAux[ SERVER_SIZE_DIRECTIVES + 1 ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size",
	"location"
};

std::string	Server::_allowedSimpleDirectivesAux[ SERVER_SIZE_SIMPLE_DIRECTIVES + 1 ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size"
};

std::string	Server::_allowedComplexDirectivesAux[ SERVER_SIZE_COMPLEX_DIRECTIVES + 1 ] = 
{
	"location"
};

ConstStringVector	Server::allowedDirectives( _allowedDirectivesAux, \
											_allowedDirectivesAux \
											+ SERVER_SIZE_DIRECTIVES );

ConstStringVector	Server::allowedSimpleDirectives( \
											_allowedSimpleDirectivesAux, \
	   										_allowedSimpleDirectivesAux \
											+ SERVER_SIZE_SIMPLE_DIRECTIVES );

ConstStringVector	Server::allowedComplexDirectives( \
											_allowedComplexDirectivesAux, \
											_allowedComplexDirectivesAux \
											+ SERVER_SIZE_COMPLEX_DIRECTIVES );
