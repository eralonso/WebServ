/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 14:16:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/PendingCgiTask.hpp"
#include "../../inc/Utils.hpp"

PendingCgiTask::PendingCgiTask()
{
}
PendingCgiTask::PendingCgiTask(pid_t pid, Request *request, int fd) :
	pid(pid), request(request), fd(fd)
{
	timestamp = std::clock();  
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

Request* PendingCgiTask::getRequest() const
{
	return request;
}

std::clock_t	PendingCgiTask::getTimestamp() const
{
	return timestamp;
}

bool	PendingCgiTask::isTimeout(std::clock_t toDuration) const
{
	std::clock_t now = std::clock();
	std::clock_t duration = now - timestamp;
	return (duration < toDuration);
}

int			PendingCgiTask::getFd() const
{
	return fd;
}

std::string PendingCgiTask::getTaskOutput()
{
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	size_t bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read == BUFFER_SIZE)
	{
		buf[BUFFER_SIZE] = 0;
		resBody += std::string(buf);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		Log::Error(std::string("Read from child failed"));
	buf[bytes_read] = 0;
	resBody += std::string(buf);
	close(fd);
	return resBody;
}

void PendingCgiTask::applyTaskOutputToReq()
{
	char buf[BUFFER_SIZE + 1];
	std::string	resBody;
	ssize_t bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read == BUFFER_SIZE)
	{
		buf[bytes_read] = 0;
		resBody += std::string(buf);
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	close(fd);
	if (bytes_read < 0)
	{
		Log::Error(std::string("Read from child failed"));
		Log::Error(std::string("read" + resBody));
		buf[0] = 0;
		return ;
	}
	buf[bytes_read] = 0;
	resBody += std::string(buf);
	request->setCgiOutput(resBody);
}