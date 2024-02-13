/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 11:47:49 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 11:12:53 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EVENTS_HPP_
# define _EVENTS_HPP_

# include <sys/event.h>
# include <unistd.h>

# include "Event.hpp"
# include "Utils.hpp"
# include "WSSignals.hpp"
# include "Log.hpp"
// # include "Receptionist.hpp"

class EventsTarget;
class Receptionist;

class Events
{
private:
	int		kq;
public:
	Events( void );
	~Events( void );
public:
	bool	isCreate( void ) const;
	int		setEventFileSaved( EventsTarget *et, int fd );
	int		setEventRead( EventsTarget* et, int fd );
	int		setEventWrite( EventsTarget *et, int fd );
	int		setEventTimer( EventsTarget *et, int ident, long period, bool oneShot );
	int		setEventProcExit( EventsTarget *et, pid_t pid, long timeout );
	int		enableEventRead( EventsTarget* et, int fd, bool enable );
	int		enableEventWrite( EventsTarget *et, int fd, bool enable );
	int		deleteEventProcExit( EventsTarget* et, pid_t pid );
	int		deleteEventProcTimeout( EventsTarget* et, pid_t pid );
	int		loopEvents( void );
};

#endif
