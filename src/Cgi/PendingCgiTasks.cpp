/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:32:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 19:10:09 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PendingCgiTasks.hpp"

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
	size_t tot;
	
	tot = this->erase( pid );
	return ( tot > 0 );
}
