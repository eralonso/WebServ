/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSSignals.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:04:00 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/29 17:03:20 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>

WSSignals::WSSignals( void ) {}
WSSignals::~WSSignals( void ) {}
WSSignals::WSSignals( const WSSignals& ) {}
WSSignals& WSSignals::operator=( const WSSignals& ) { return ( *this ); }

bool	WSSignals::isSig = false;

void	WSSignals::sighandler( int sig )
{
	Log::Info( "Signal detected: " + SUtils::longToString( sig ) );
	if ( sig == SIGINT || sig == SIGQUIT )
		WSSignals::isSig = true;
}

void	WSSignals::signalHandler( void )
{
	signal( SIGINT, sighandler );
	signal( SIGQUIT, sighandler );
	signal( SIGPIPE, sighandler );
}
