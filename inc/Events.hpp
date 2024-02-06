/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:47:49 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/06 09:26:28 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EVENTS_HPP_
# define _EVENTS_HPP_

# include <sys/event.h>

# include "Event.hpp"

class EventsTarget;

class Events
{
private:
	int		kq;
	bool	successCreate;
public:
	Events( void );
	~Events( void );
public:
	bool	is_create( void ) const;
	int		setEventFileSaved( EventsTarget *et, int fd );
	int		setEventWrite( EventsTarget *et, int fd );
	int		setEventTimer( EventsTarget *et, int ident, long period, bool oneShot );
	int		setEventProcExit( EventsTarget *et, pid_t pid, long timeout );
	int		loopEvents( void );
};

#endif
