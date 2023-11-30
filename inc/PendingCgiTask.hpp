/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:14 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 18:09:15 by omoreno-         ###   ########.fr       */
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
	Request*		request;
	std::clock_t	timestamp;
	int				fd;
public:
	PendingCgiTask();
	PendingCgiTask(pid_t pid, Request* request,	int fd);
	PendingCgiTask(const PendingCgiTask& b);
	PendingCgiTask& operator=(const PendingCgiTask& b);
	~PendingCgiTask();
	pid_t									getPid() const;
	Request*								getRequest() const;
	clock_t									getTimestamp() const;
	bool									isTimeout(clock_t toDuration) const;
	int										getFd() const;
	std::string								getTaskOutput();
	void									applyTaskOutputToReq();
};

#endif