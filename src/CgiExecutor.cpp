/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 14:39:37 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <CgiExecutor.hpp>
#define FDIN 0
#define FDOUT 1

CgiExecutor::CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env) :
		binary(binary), argument(argument), request(request)
{
	const std::string	reqBody;
	char				*envPath;
	argv[0] = (char *)binary.c_str();
	argv[1] = (char *)argument.c_str();
	argv[2] = NULL;
	childEnv[0] = NULL;
	childEnv[9] = NULL;
}

CgiExecutor::~CgiExecutor()
{
	;
}

void CgiExecutor::onFailFork(void)
{
		Log::Error("fork: failed create child process");
		close(fdToChild[FDIN]);
		close(fdToChild[FDOUT]);
		close(fdFromChild[FDIN]);
		close(fdFromChild[FDOUT]);
		throw;
}

void CgiExecutor::onFailToChildPipeOpen(void)
{
	Log::Error("pipe: failed to open pipe to child");
	throw;
}

void CgiExecutor::onFailFromChildPipeOpen(void)
{
	Log::Error("pipe: failed to open pipe from child");
	close(fdToChild[FDIN]);
	close(fdToChild[FDOUT]);
	throw;
}

void CgiExecutor::onChildProcess(void)
{
		//On child
		close(fdToChild[FDOUT]);
		close(fdFromChild[FDIN]);
		if (dup2(fdToChild[FDIN], FDIN))
			Log::Error(std::string("dup2: fdToChild[0] to 0 failed"));
		if (dup2(fdFromChild[FDOUT], FDOUT));
			Log::Error(std::string("dup2: fdFromChild[1] to 1 failed"));
		close(fdToChild[FDIN]);
		close(fdFromChild[FDOUT]);
		execve(binary.c_str(), argv, childEnv);
		close(FDIN);
		close(FDOUT);
		Log::Error(std::string("execve: Not found binary ") + binary);
		return; //exit(1) not alloed?
}

void CgiExecutor::onParentProcess(pid_t childPid)
{
	std::string	body = request.getBody();
	char *reqBody = (char *)body.c_str();
	size_t reqBodySize = body.length();
	close(fdToChild[FDIN]);
	close(fdFromChild[FDOUT]);
	write(fdToChild[FDOUT], reqBody, reqBodySize);
	close(fdToChild[FDOUT]);
	new PendingCgiTask(childPid, request, fdFromChild[FDIN]);
	//waitpid(childPid, NULL, 0);
}

std::string CgiExecutor::getChildOutput(PendingCgiTask *task)
{
	if (!task)
		return (std::string(""));
	int fd = task->getFd();
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	size_t bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read == BUFFER_SIZE)
	{
		buf[BUFFER_SIZE] = NULL;
		resBody += std::string(buf);
		size_t bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		Log::Error(std::string("Read from child failed"));
	buf[bytes_read] = NULL;
	resBody += std::string(buf);
	close(fd);
	return resBody;
}

int CgiExecutor::execute(void)
{
	if (pipe(fdToChild))
		onFailToChildPipeOpen();
	if (pipe(fdFromChild))
		onFailFromChildPipeOpen();
	pid_t pid = fork();
	if (pid < 0)
		onFailFork();
	if(pid == 0)
		onChildProcess();
	onParentProcess(pid);
	return 0;
	// return getChildOutput();
}

PendingCgiTask *CgiExecutor::getCompletedTask()
{
	pid_t	pid;
	pid = waitpid(-1, NULL, WNOHANG);
	if (pid < 1)
		return nullptr;
	return &(pendingTasks[pid]);
}

PendingCgiTask *CgiExecutor::getTimeoutedTask(std::chrono::duration<double> to)
{
	pid_t	pid;
	PendingCgiTasks::iterator it = pendingTasks.begin();
	PendingCgiTasks::iterator ite= pendingTasks.end();
	while (it != ite)
	{
		if(it->second.isTimeout(to));
		return (&(it->second));
		it++;
	}
	return nullptr;
}

std::string CgiExecutor::getCompletedTaskOutput(void)
{
	PendingCgiTask *task = getCompletedTask();
	if (task)
	{
		std::string ret = task->getTaskOutput();
		pendingTasks.eraseTask(task);
		return (ret);
	}
	return std::string();
}

size_t	CgiExecutor::purgeTimeoutedTasks(std::chrono::duration<double> to,
	size_t max)
{
	size_t i = 0;
	PendingCgiTask *task = nullptr;
	while (i < max && (task = getTimeoutedTask(to)))
	{
		pendingTasks.eraseTask(task);
		i++;
	}
	return (i);
}


