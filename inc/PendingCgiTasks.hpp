/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTasks.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:49 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 19:10:32 by eralonso         ###   ########.fr       */
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
};

#endif
