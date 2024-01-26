/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/26 11:23:41 by omoreno-         ###   ########.fr       */
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
	std::runtime_error		exceptBinNotAccess;
	std::runtime_error		exceptScriptNotAccess;
	std::runtime_error		exceptOther;
	static PendingCgiTasks	pendingTasks;
	std::string				binary;
	std::string				argument;
	StringVector			envVars;
	Request&				request;
	int						fdToChild[ 2 ];
	int						fdFromChild[ 2 ];
	char					*argv[ 3 ];
	char					**childEnv;
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
