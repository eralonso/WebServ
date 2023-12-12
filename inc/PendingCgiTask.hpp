/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:14 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/12 14:40:59 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASK_HPP_
# define _PENDINGCGITASK_HPP_
# include <unistd.h>
# include <ctime>
# include "Request.hpp"

class PendingCgiTask
{
private:
	pid_t			pid;
	Request&		request;
	std::clock_t	timestamp;
	int				fd;
	bool			markedToDelete;
public:
	PendingCgiTask();
	PendingCgiTask(pid_t pid, Request& request,	int fd);
	PendingCgiTask(const PendingCgiTask& b);
	PendingCgiTask& operator=(const PendingCgiTask& b);
	~PendingCgiTask();
	pid_t									getPid() const;
	Request&								getRequest() const;
	clock_t									getTimestamp() const;
	bool									isMarkedToDelete() const;
	bool									isTimeout(double toDuration, bool logInfo) const;
	int										getFd() const;
	std::string								getTaskOutput();
	void									applyTaskOutputToReq();
	void 									closeReadFd();
	void									killPendingTask();
	void									setMarkedToDelete(bool value);
};

#endif