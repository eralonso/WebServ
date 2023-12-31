/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSPoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:07:12 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/05 17:01:07 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WSPOLL_HPP_
# define _WSPOLL_HPP_

# include <poll.h>
# include <unistd.h>

# include <iostream>

# include "Defines.hpp"
# include "Utils.hpp"

class WSPoll
{
	private:
		struct pollfd	*_polls;
		unsigned int	_maxSize;
		unsigned int	_size;
		unsigned int	_serverSizeFd;
	private:
		WSPoll( void );
	public:
		WSPoll( unsigned int maxSize );
		WSPoll( const WSPoll& wspl );
		~WSPoll( void );
		WSPoll	operator=( const WSPoll& wspl );
	public:
		const struct pollfd&	operator[]( unsigned int pos ) const;
		struct pollfd&			operator[]( unsigned int pos );
		const struct pollfd&	operator[]( socket_t fd ) const;
		struct pollfd&			operator[]( socket_t fd );
	public:
		unsigned int	getMaxSize( void ) const;
		unsigned int	getSize( void ) const;
		unsigned int	getServerSizeFd( void ) const;
	public:
		void		deletePolls( void );
		bool		addPollfd( socket_t fd, int events, int revents, int type );
		int			checkPollReturn( int ret ) const;
		void		restartPoll( unsigned int pos );
		void		restartPoll( unsigned int start, unsigned int end );
		void		closePoll( unsigned int pos );
		void		closePoll( unsigned int start, unsigned int end );
		void		closePoll( socket_t fd );
		void		compressPolls( unsigned int start );
		int			wait( int timeout );
		socket_t	isNewClient( void );
		socket_t	getPerformClient( void );
		void		allowPollWrite(socket_t fd, bool value);
};

#endif
