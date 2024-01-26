/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSSignals.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:04:00 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/26 11:03:44 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>

WSSignals::WSSignals( void ) {}
WSSignals::~WSSignals( void ) {}
WSSignals::WSSignals( const WSSignals& ) {}
WSSignals& WSSignals::operator=( const WSSignals& ) { return ( *this ); }

bool	WSSignals::isSig = false;

void	WSSignals::sighandler( int )
{
	Log::Info( "Signal detected" );
	WSSignals::isSig = true;
}

void	WSSignals::signalHandler( void )
{
	signal( SIGINT, sighandler );
	signal( SIGQUIT, sighandler );
}
