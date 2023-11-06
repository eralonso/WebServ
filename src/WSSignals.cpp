/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSSignals.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:04:00 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 12:39:22 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>

bool	WSSignals::isSig = false;

void	WSSignals::sighandler( int )
{
	Log::Info( "Signal detected" );
	isSig = true;
}

void	WSSignals::signalHandler( void )
{
	signal( SIGINT, sighandler );
	signal( SIGQUIT, sighandler );
}

