/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/19 17:53:29 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PendingCgiTasks.hpp"
#include <Utils.hpp>

PendingCgiTasks::PendingCgiTasks( void ) {}

PendingCgiTasks::PendingCgiTasks( const PendingCgiTasks &b ): \
							std::map< pid_t,PendingCgiTask >( b ) {}

PendingCgiTasks& PendingCgiTasks::operator=( const PendingCgiTasks& b )
{
	if ( this != &b )
		std::map< pid_t,PendingCgiTask >::operator=( b );
	return ( *this );
}

PendingCgiTasks::~PendingCgiTasks( void ) {}

int	PendingCgiTasks::appendTask( PendingCgiTask task )
{
	this->insert( std::pair< pid_t, PendingCgiTask >( task.getPid(), task ) );
	return ( 0 );
}

int PendingCgiTasks::eraseTask( pid_t pid )
{
	if (pid <= 0 and size() == 1)
	{
		Log::Info("ClearMap PendingCgiTasks");
		clear();
		return (1);
	}
	Log::Info("Erasing PendingCgiTask with pid: " + SUtils::longToString(pid));
	size_t tot;
	
	tot = this->erase( pid );
	return ( tot > 0 );
}
