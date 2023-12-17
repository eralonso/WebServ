/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PendingCgiTask.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:14 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 19:04:54 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PENDINGCGITASK_HPP_
# define _PENDINGCGITASK_HPP_

# include <unistd.h>
# include <ctime>

# include "Request.hpp"
# include "Utils.hpp"

class PendingCgiTask
{
private:
	pid_t			pid;
	Request&		request;
	std::clock_t	timestamp;
	int				fd;
public:
	PendingCgiTask( void );
	PendingCgiTask( pid_t pid, Request& request, int fd );
	PendingCgiTask( const PendingCgiTask& b );
	~PendingCgiTask( void );
	PendingCgiTask&	operator=( const PendingCgiTask& b );
	pid_t			getPid( void ) const;
	Request&		getRequest( void ) const;
	clock_t			getTimestamp( void ) const;
	bool			isTimeout( clock_t toDuration ) const;
	int				getFd( void ) const;
	std::string		getTaskOutput( void );
	void			applyTaskOutputToReq( void );
};

#endif
