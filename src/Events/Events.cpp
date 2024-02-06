/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:24:58 by eralonso          #+#    #+#             */

/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>
#include "Events.hpp"
#include "EventsTarget.hpp"

Events::Events( void )
{
	this->kq = kqueue();
}

Events::~Events( void )
{
	close( this->kq );
}

bool	Events::isCreate( void ) const
{
	return ( kq != -1 );
}

int	Events::setEventFileSaved( EventsTarget* et, int fd )
{
	int				ret;
	struct kevent	fileEvent;

	EV_SET(&fileEvent, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, et);
	ret = kevent(kq, &fileEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent FileSaved register" );
	return 0;
}

int	Events::setEventWrite( EventsTarget* et, int fd )
{
	int				ret;
	struct kevent	writeEvent;

	EV_SET(&writeEvent, fd, EVFILT_WRITE, EV_ADD | EV_EOF, 0, 0, et);
	ret = kevent(kq, &writeEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Write register" );
	return 0;
}

int	Events::setEventRead( EventsTarget* et, int fd )
{
	int				ret;
	struct kevent	readEvent;

	EV_SET(&readEvent, fd, EVFILT_READ, EV_ADD | EV_EOF, 0, 0, et);
	ret = kevent(kq, &readEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Read register" );
	return ( ret );
}

int	Events::setEventTimer( EventsTarget* et, int ident, long period, bool oneShot )
{
	int				ret;
	struct kevent	timeEvent;

	EV_SET(&timeEvent, ident, EVFILT_TIMER, EV_ADD | EV_CLEAR | \
			(oneShot ? EV_ONESHOT : 0), 0, period, et);
	ret = kevent(kq, &timeEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Timer register" );
	return ( ret );
}

int	Events::setEventProcExit( EventsTarget* et, pid_t pid, long timeout )
{
	int				ret;
	struct kevent	procEvent[ 2 ];

	EV_SET(procEvent, pid, EVFILT_PROC, EV_ADD | EV_CLEAR, NOTE_EXIT, 0, et);
	EV_SET(procEvent + 1, pid, EVFILT_TIMER, EV_ADD | EV_CLEAR | EV_ONESHOT, \
		NOTE_SECONDS, timeout, et);
	ret = kevent(kq, procEvent, 2, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Processs Exit register" );
	return ( ret );
}

int		Events::enableEventRead( EventsTarget* et, int fd, bool enable )
{
	int				ret;
	struct kevent	readEvent;

	EV_SET(&readEvent, fd, EVFILT_READ, EV_ADD | EV_EOF | ( enable ? EV_ENABLE : EV_DISABLE ), 0, 0, et);
	ret = kevent(kq, &readEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Read register" );
	return 0;
}

int		Events::enableEventWrite( EventsTarget* et, int fd, bool enable )
{
	int				ret;
	struct kevent	writeEvent;

	EV_SET(&writeEvent, fd, EVFILT_WRITE, EV_ADD | EV_EOF | ( enable ? EV_ENABLE : EV_DISABLE ), 0, 0, et);
	ret = kevent(kq, &writeEvent, 1, NULL, 0, NULL);
	if (ret == -1)
		Log::Error( "kevent Write register" );
	return 0;
}

int	Events::loopEvents( void )
{
	int				ret = 0;
	Event			tevent;
	EventsTarget	*et = NULL;

	while ( ret > 0 && WSSignals::isSig == false )
	{
		ret = kevent( kq, NULL, 0, &tevent, 1, NULL );
		if (ret > 0)
		{
			if ( tevent.flags & EV_ERROR )
			{
				Log::Error( "Attempting catch an event with ident [ " \
					+ SUtils::longToString( tevent.ident ) + " ]" );
				return ( -1 );
			}
			et = static_cast< EventsTarget * >( tevent.udata );
			if ( et )
				et->onEvent( tevent );
		}
	}
	return ( ret < 0 );
}
