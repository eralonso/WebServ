/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/22 14:35:27 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/PendingCgiTasks.hpp"

PendingCgiTasks::PendingCgiTasks()
{
}

PendingCgiTasks::PendingCgiTasks(const PendingCgiTasks &b) :
	map<pid_t,PendingCgiTask>(b)
{
}

PendingCgiTasks& PendingCgiTasks::operator=(const PendingCgiTasks& b)
{
	map<pid_t,PendingCgiTask>::operator=(b);
	return (*this);	
}

PendingCgiTasks::~PendingCgiTasks()
{
}

int PendingCgiTasks::appendTask(const PendingCgiTask &task)
{
	insert(std::pair<pid_t, PendingCgiTask>(task.getPid(), task));
	return 0;
}

int PendingCgiTasks::eraseTask(const PendingCgiTask* task)
{
	if (task)
	{
		pid_t pid = task->getPid();
		size_t tot = erase(pid);
		return (tot > 0);
	}
	return (0);
}
