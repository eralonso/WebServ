/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 12:11:33 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGIEXECUTOR_HPP_
# define _CGIEXECUTOR_HPP_

# include <string>
# include <vector>
# include <unistd.h>
# include <sys/wait.h>

# include <cerrno>
# include <ctime>
# include <exception>

# include "Utils.hpp"
# include "Request.hpp"
# include "Router.hpp"
# include "Client.hpp"
# include <ServerFinder.hpp>

# define FDIN 0
# define FDOUT 1
# define CGI_TO 2

class CgiExecutor
{
private:
	std::string				binary;
	std::string				argument;
	StringVector			envVars;
	Request&				request;
	int						fdToChild[ 2 ];
	int						fdFromChild[ 2 ];
	char					*argv[ 3 ];
	char					**childEnv;
	CgiExecutor( const CgiExecutor& b);
	CgiExecutor& operator=( const CgiExecutor& );
	void					onFailFork(void);
	void					onFailToChildPipeOpen(void);
	void					onFailFromChildPipeOpen(void);
	char **					getEnvVarList(void);
	void 					onChildProcess(void);
	void					onParentProcess(pid_t childPid);
	bool					checkFileReadable(std::string file);
	bool 					checkFileExecutable(std::string file);
public:
	CgiExecutor( Request& request );
	~CgiExecutor( void );
	int						execute( void );
	void					pushEnvVar( const std::string& variable, \
								const std::string& value );
};

#endif
