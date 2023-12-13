/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:07:44 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/07 15:12:14 by eralonso         ###   ########.fr       */
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
	"upload_store",
	"location"
};

StringVector	Server::allowedDirectives( _allowedDirectivesAux, \
											_allowedDirectivesAux \
											+ SERVER_SIZE_DIRECTIVES );
