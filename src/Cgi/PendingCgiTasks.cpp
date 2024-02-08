/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 10:22:16 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PendingCgiTasks.hpp"
#include <Utils.hpp>

PendingCgiTasks::PendingCgiTasks( void ) {}

PendingCgiTasks::PendingCgiTasks( const PendingCgiTasks &b ): \
							CgiTaskMap( b ) {}

PendingCgiTasks& PendingCgiTasks::operator=( const PendingCgiTasks& b )
{
	if ( this != &b )
		CgiTaskMap::operator=( b );
	return ( *this );
}

PendingCgiTasks::~PendingCgiTasks( void ) {}

int	PendingCgiTasks::appendTask( PendingCgiTask task )
{
	this->insert( CgiTaskMapPair( task.getPid(), task ) );
	return ( 0 );
}

int PendingCgiTasks::eraseTask( pid_t pid )
{
	size_t	tot;

	if ( pid <= 0 && this->size() == 1 )
	{
		// Log::Info( "ClearMap PendingCgiTasks" );
		this->clear();
		return ( 1 );
	}
	// Log::Info( "Erasing PendingCgiTask with pid: "
	// + SUtils::longToString( pid ) );
	tot = this->erase( pid );
	return ( tot > 0 );
}

int PendingCgiTasks::eraseTask(Request *req)
{
	pid_t pid = findPid(req);
	if (pid > 0)
		eraseTask(pid);
	return (0);
}

pid_t PendingCgiTasks::findPid(Request *req)
{
	PendingCgiTasks::iterator it = begin();	
	PendingCgiTasks::iterator ite = end();
	while (it != ite)
	{
		if (&(it->second.getRequest()) == req)
			return (it->first);
		it++;
	}
	return pid_t(0);
}

pid_t PendingCgiTasks::findPid(Client *cli)
{
	PendingCgiTasks::iterator it = begin();	
	PendingCgiTasks::iterator ite = end();
	while (it != ite)
	{
		if ((it->second.getRequest().getClient()) == cli)
			return (it->first);
		it++;
	}
	return pid_t(0);
}
