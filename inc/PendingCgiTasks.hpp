/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:49 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/16 18:09:23 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASKS_HPP_
# define _PENDINGCGITASKS_HPP_
# include <map>
# include "PendingCgiTask.hpp"


class PendingCgiTasks : public std::map<pid_t, PendingCgiTask>
{
private:
public:
	PendingCgiTasks();
	PendingCgiTasks(const PendingCgiTasks& b);
	PendingCgiTasks& operator=(const PendingCgiTasks& b);
	~PendingCgiTasks();
	int appendTask(const PendingCgiTask& task);
	int eraseTask(const PendingCgiTask* task);
};

#endif