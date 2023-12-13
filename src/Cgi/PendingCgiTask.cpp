/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/13 11:23:19 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "../../inc/PendingCgiTask.hpp"
#include "../../inc/Utils.hpp"
#include "PendingCgiTask.hpp"

PendingCgiTask::PendingCgiTask() : request(*new Request())
{
	timestamp = std::clock();
}

PendingCgiTask::PendingCgiTask(pid_t pid, Request& request, int fd) :
	pid(pid), request(request), fd(fd), markedToDelete(false)
{
	timestamp = std::clock();
	Log::Info("Task pid " + SUtils::longToString(pid) + " time: " + SUtils::longToString(timestamp));
}

PendingCgiTask::PendingCgiTask(const PendingCgiTask &b) :
	pid(b.pid), request(b.request), timestamp(b.timestamp), fd(b.fd) 
{
}

PendingCgiTask &PendingCgiTask::operator=(const PendingCgiTask &b)
{
	pid = b.pid;
	request = b.request;
	fd = b.fd;
	timestamp = b.timestamp;
	return (*this);
}

PendingCgiTask::~PendingCgiTask()
{
}

pid_t PendingCgiTask::getPid() const
{
	return pid;
}

Request& PendingCgiTask::getRequest() const
{
	return request;
}

std::clock_t	PendingCgiTask::getTimestamp() const
{
	return timestamp;
}

bool PendingCgiTask::isMarkedToDelete() const
{
	return markedToDelete;
}

bool PendingCgiTask::isTimeout(double toDuration, bool logInfo) const
{
	std::clock_t now = std::clock();
	if (logInfo)
	{
		// Log::Info("isTimeout at time: " + SUtils::longToString(now));
		// Log::Info("isTimeout over timestamp: " + SUtils::longToString(timestamp));
		Log::Info("Cgi Timeout: " + SUtils::longToString(static_cast<long>(toDuration*1000)) + "ms");
	}
	long duration = (now - timestamp);
	if (logInfo)
		Log::Info("exceeded at duration: " + SUtils::longToString(duration) + "ms");
	return (duration > toDuration * 1000);
}

int			PendingCgiTask::getFd() const
{
	return fd;
}

std::string PendingCgiTask::getTaskOutput()
{
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	int	bytes_read = read(fd, buf, BUFFER_SIZE);

	while (bytes_read == BUFFER_SIZE)
	{
		buf[BUFFER_SIZE] = 0;
		resBody += std::string(buf, bytes_read);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		Log::Error(std::string("Read from child failed"));
	buf[bytes_read] = 0;
	if (bytes_read > 0)
		resBody += std::string(buf, bytes_read);
	close(fd);
	return resBody;
}

void PendingCgiTask::applyTaskOutputToReq()
{
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	int bytes_read = read(fd, buf, BUFFER_SIZE);

	while (bytes_read == BUFFER_SIZE)
	{
		buf[bytes_read] = 0;
		resBody += std::string(buf, bytes_read);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	close(fd);
	setMarkedToDelete(true);
	if (bytes_read < 0)
	{
		Log::Error(std::string("Read from child failed"));
		buf[0] = 0;
		return ;
	}
	buf[bytes_read] = 0;
	resBody += std::string(buf, bytes_read);
	Log::Success(std::string("read" + resBody));
	request.setCgiOutput(resBody);
}

void	PendingCgiTask::closeReadFd()
{
	if (fd >= 0)
		close(fd);
	fd = -1;
}

void	PendingCgiTask::killPendingTask()
{
	if (pid > 0)
	{
		markedToDelete = true;
		kill(pid, SIGKILL);
	}
	pid = 0;
	if (fd >= 0)
		close(fd);
	fd = -1;
}

void PendingCgiTask::setMarkedToDelete(bool value)
{
	markedToDelete = value;
}
