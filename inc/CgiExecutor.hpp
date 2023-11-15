/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 17:42:47 by omoreno-         ###   ########.fr       */
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
	int					fdToChild[2];
	int					fdFromChild[2];
	char				*argv[3];
	char				*childEnv[10];
	std::string 		getChildOutput(void);
	void				onFailFork(void);
	void				onFailToChildPipeOpen(void);
	void				onFailFromChildPipeOpen(void);
	void				onChildProcess(void);
	void				onParentProcess(t_pid childPid);
public:
	CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env);
	~CgiExecutor();
	std::string execute(void);
};

#endif