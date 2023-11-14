/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:47:25 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGIEXECUTOR_HPP_
# define _CGIEXECUTOR_HPP_
# include <string>
# include <Request.hpp>

class CgiExecutor
{
private:
	const std::string&	binary;
	const std::string&	argument;
	Request				request;
	char				*envPath;
public:
	CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env);
	~CgiExecutor();
	std::string Execute(void);
};

#endif