/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 15:23:31 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/06 08:22:44 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( Events *bEvs ): EventsTarget( bEvs ) {}

Client::~Client( void ) {}

int	Client::readFromSocket( Event& tevent )
{
	int	amountRecv;

	if ( tevent.flags & EV_EOF )
		return ( EV_EOF );
	amountRecv = recv( socketFd, this->buffer, READ_BUFFER_SIZE, MSG_DONTWAIT );
	return ( amountRecv );
}

int	Client::writeToSocket( Event& tevent )
{
	int	amountSent;

	if ( tevent.flags & EV_EOF )
		return ( EV_EOF );
	amountSent = send( socketFd, this->buffer, SEND_BUFFER_SIZE, MSG_DONTWAIT );
	return ( amountSent );
}

int	Client::onEvent( Event &tevent )
{
	if ( tevent.flags & EV_ERROR )
		errx(EXIT_FAILURE, "Event error:	id: '%ld' filt: '%hd' flags: '%hd' fflags: '%ud' data:'%ld'\n",
			tevent.ident, tevent.filter, tevent.flags, tevent.fflags, tevent.data);
	else
	{
		if ( tevent.filter & EVFILT_READ )
			return ( readFromSocket( tevent ) );
		else if ( tevent.filter & EVFILT_WRITE )
			return ( writeToSocket( tevent ) );
	}
	return ( 0 );
}
