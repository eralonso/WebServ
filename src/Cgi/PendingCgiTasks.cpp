/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 16:58:51 by omoreno-         ###   ########.fr       */
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

int PendingCgiTasks::appendTask(PendingCgiTask task)
{
	insert(std::pair<pid_t, PendingCgiTask>(task.getPid(), task));
	return 0;
}

int PendingCgiTasks::eraseTask(pid_t pid)
{
	size_t tot = erase(pid);
	return (tot > 0);
}
