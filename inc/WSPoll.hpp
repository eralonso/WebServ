/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSPoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:07:12 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/26 19:32:13 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WSPOLL_H__
# define __WSPOLL_H__

#include <poll.h>
#include "Defines.hpp"

class WSPoll
{
	private:
		struct pollfd	*_polls;
		unsigned int	_maxSize;
	private:
		WSPoll( void );
	public:
		WSPoll( unsigned int maxSize );
		WSPoll( const WSPoll& wspl );
		~WSPoll( void );
		WSPoll	operator=( const WSPoll& wspl );
	public:
		void	deletePolls( void );
		void	setPollFd( unsigned int pos, socket_t fd, int events, int revents );
		int		checkPollReturn( int ret );
		void	restartPoll( unsigned int pos );
		void	restartPoll( unsigned int start, unsigned int end );
		void	closePoll( unsigned int pos );
		void	closePoll( int start, int end );
		void	reallocPolls( int start );
};

#endif
