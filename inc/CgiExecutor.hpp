/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:34 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 18:25:03 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGIEXECUTOR_HPP_
# define _CGIEXECUTOR_HPP_

# include <string>
# include <unistd.h>
# include <sys/wait.h>

# include <cerrno>
# include <ctime>

# include "Utils.hpp"
# include "Request.hpp"
# include "PendingCgiTask.hpp"
# include "PendingCgiTasks.hpp"
# include "Router.hpp"
# include "Client.hpp"

# define FDIN 0
# define FDOUT 1
# define CGI_TO 2000

class CgiExecutor
{
private:
	static PendingCgiTasks	pendingTasks;
	std::string				binary;
	std::string				argument;
	Request&				request;
	//char					*envPath;
	int						fdToChild[ 2 ];
	int						fdFromChild[ 2 ];
	char					*argv[ 3 ];
	char					*childEnv[ 10 ];
	static std::string		getChildOutput( PendingCgiTask *task );
	void					onFailFork( void );
	void					onFailToChildPipeOpen( void );
	void					onFailFromChildPipeOpen( void );
	void					onChildProcess( void );
	void					onParentProcess( pid_t childPid );
public:
	CgiExecutor( Request& request, char **env );
	~CgiExecutor( void );
	int						execute( void );
	static PendingCgiTask	*getCompletedTask( void );
	static PendingCgiTask	*getTimeoutedTask( clock_t to );
	static std::string		getCompletedTaskOutput( void );
	static size_t			purgeTimeoutedTasks( clock_t to, size_t max );
	static void				attendPendingCgiTasks( void );
};

#endif
