/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:49 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/03 16:01:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASKS_HPP_
# define _PENDINGCGITASKS_HPP_

# include <map>

# include "PendingCgiTask.hpp"

class PendingCgiTasks: public std::map< pid_t, PendingCgiTask >
{
private:
public:
	PendingCgiTasks( void );
	PendingCgiTasks( const PendingCgiTasks& b );
	~PendingCgiTasks( void );
	PendingCgiTasks&	operator=( const PendingCgiTasks& b );
	int					appendTask( PendingCgiTask task );
	int					eraseTask( pid_t pid );
	int					eraseTask( Request *req );
	pid_t				findPid( Request *req );
};

#endif
