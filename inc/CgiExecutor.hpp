/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/30 16:16:25 by codespace        ###   ########.fr       */
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
# include "PendingCgiTask.hpp"
# include "PendingCgiTasks.hpp"
# include "Router.hpp"
# include "Client.hpp"
# include <ServerFinder.hpp>

# define FDIN 0
# define FDOUT 1
# define CGI_TO 2

class CgiExecutor
{
private:
	static PendingCgiTasks	pendingTasks;
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
	static std::string		getChildOutput(PendingCgiTask *task);
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
	static PendingCgiTask	*getCompletedTask( void );
	static PendingCgiTask	*getTimeoutedTask( double to );
	static PendingCgiTask	*getMarkedToDeleteTask( void );
	static size_t			purgeTimeoutedTasks( double to, size_t max );
	static void				attendPendingCgiTasks( void );	
	static void				checkCompletedTasks( void );
	static void				checkTimeoutedTasks( void );
	static void				checkMarkedToDeleteTasks( void );
	static size_t			getPendingTasksSize( void );
	static int				purgeDiscardedRequest(Request *req);
	static pid_t			findClientPendingPid(Client * cli);
	void					pushEnvVar( const std::string& variable, \
								const std::string& value );
};

#endif
