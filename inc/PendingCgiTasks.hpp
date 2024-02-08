/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:49 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 10:25:29 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASKS_HPP_
# define _PENDINGCGITASKS_HPP_

# include <map>

# include "PendingCgiTask.hpp"

typedef std::map< pid_t, PendingCgiTask >	CgiTaskMap;
typedef std::pair< pid_t, PendingCgiTask >	CgiTaskMapPair;

class PendingCgiTasks: public CgiTaskMap
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
	pid_t				findPid( Client *cli );
};

#endif
