/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:47:30 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CgiExecutor.hpp>

CgiExecutor::CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env) :
		binary(binary), argument(argument), request(request)
{
	const std::string	reqBody;
	char				*envPath;
}

CgiExecutor::~CgiExecutor()
{
	;
}

std::string CgiExecutor::Execute(void)
{
	;
}

