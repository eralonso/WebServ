/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:14 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 13:52:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASK_HPP_
# define _PENDINGCGITASK_HPP_
# include <unistd.h>
# include <chrono>
# include <Request.hpp>

class PendingCgiTask
{
private:
	pid_t									pid;
	Request&								request;
	std::chrono::steady_clock::time_point	timestamp;
	int										fd;
public:
	PendingCgiTask(pid_t pid, Request& request,	int fd);
	PendingCgiTask(const PendingCgiTask& b);
	PendingCgiTask& operator=(const PendingCgiTask& b);
	~PendingCgiTask();
	pid_t									getPid() const;
	Request&								getRequest() const;
	std::chrono::steady_clock::time_point	getTimestamp() const;
	bool		isTimeout(std::chrono::duration<double> toDuration) const;
	int										getFd() const;
	std::string								getTaskOutput();
};

#endif