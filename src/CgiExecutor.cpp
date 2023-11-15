/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:58:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 17:56:37 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <CgiExecutor.hpp>
#include <Utils.hpp>
#define BUFFER_SIZE 99
#define FDIN 0
#define FDOUT 1

CgiExecutor::CgiExecutor(const std::string& binary, const std::string& argument,
		Request& request, char **env) :
		binary(binary), argument(argument), request(request)
{
	const std::string	reqBody;
	char				*envPath;
	argv[0] = binary.str();
	argv[1] = argument.str();
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
		execve(binary.str(), argv, childEnv);
		close(FDIN);
		close(FDOUT);
		Log::Error(std::string("execve: Not found binary ") + binary);
		throw;
}

void CgiExecutor::onParentProcess(t_pid childPid)
{
	char *reqBody = body.str();
	size_t reqBodySize = body.str().lenght();
	close(fdToChild[FDIN]);
	close(fdFromChild[FDOUT]);
	write(fdToChild[FDOUT], reqBody, reqBodySize);
	close(fdToChild[FDOUT]);
	waitpid(childPid, NULL, 0);
}

std::string CgiExecutor::getChildOutput(void)
{
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	size_t bytes_read = read(fdFromChild[FDIN], buf, BUFFER_SIZE);
	while (bytes_read == BUFFER_SIZE)
	{
		buff[BUFFER_SIZE] = NULL;
		resBody += std::string(buff);
		size_t bytes_read = read(fdFromChild[FDIN], buf, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		Log::Error(std::string("Read from child failed"));
	buff[bytes_read] = NULL;
	resBody += std::string(buff);
	close(fdFromChild[FDIN]);
	return resBody;
}

std::string CgiExecutor::execute(void)
{
	if (pipe(fdToChild))
		onFailToChildPipeOpen();
	if (pipe(fdFromChild))
		onFailFromChildPipeOpen();
	pid_t pid = fork()
	if (pid < 0)
		onFailFork();
	if(pid == 0)
		onChildProcess();
	onParentProcess(pid);
	return getChildOutput();
}

