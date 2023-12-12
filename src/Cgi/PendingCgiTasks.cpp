/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/12 14:50:52 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/PendingCgiTasks.hpp"
#include <Utils.hpp>

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

int PendingCgiTasks::appendTask(PendingCgiTask task)
{
	insert(std::pair<pid_t, PendingCgiTask>(task.getPid(), task));
	return 0;
}

int PendingCgiTasks::eraseTask(pid_t pid)
{
	if (pid <= 0 and size() == 1)
	{
		Log::Info("ClearMap PendingCgiTasks");
		clear();
		return (1);
	}
	Log::Info("Erasing PendingCgiTask with pid: " + SUtils::longToString(pid));
	size_t tot = erase(pid);
	return (tot > 0);
}
