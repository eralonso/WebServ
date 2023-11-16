/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 13:53:05 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PendingCgiTask.hpp>
#include <Utils.hpp>
#define BUFFER_SIZE 99

PendingCgiTask::PendingCgiTask(pid_t pid, Request &request, int fd) :
	pid(pid), request(request), fd(fd)
{
	timestamp = std::chrono::high_resolution_clock::now();
}

PendingCgiTask::PendingCgiTask(const PendingCgiTask &b) :
	pid(b.pid), request(b.request), fd(b.fd), timestamp(b.timestamp)
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

std::chrono::steady_clock::time_point	PendingCgiTask::getTimestamp() const
{
	return timestamp;
}

bool	PendingCgiTask::isTimeout(std::chrono::duration<double> toDuration) const
{
	std::chrono::steady_clock::time_point now;
	now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = now - timestamp;
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