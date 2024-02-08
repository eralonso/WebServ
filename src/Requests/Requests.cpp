/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:58:31 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 11:01:47 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Requests.hpp"
#include <Utils.hpp>

Requests::Requests( void ) {}

Requests::~Requests( void )
{
	while ( this->size() > 0 )
		eraseRequest();
}

Requests::Requests( const Requests& b ): RequestQueue( b ) {}

Requests&	Requests::operator=( const Requests& b )
{
	if ( this != &b )
		RequestQueue::operator=( b );
	return ( *this );
}

Request	*Requests::appendRequest( Client *cli )
{
	Request	*req = new Request( cli );

	if ( !req )
		return ( NULL );
	this->push( req );
	return ( req );
}

int	Requests::eraseRequest( void )
{
	Request				*req = NULL;

	if ( this->size() > 0 )
	{
		req = this->front();
		if ( req != NULL )
		{
			this->pop();
			delete req;
			return ( 1 );
		}
	}
	return ( 0 );
}

bool	Requests::checkPendingToSend( void )
{
	size_t	pos = this->size();
	Request	*r = NULL;

	if ( pos > 0 )
	{
		r = this->front();
		if ( r->isReadyToSend() )
			return ( 1 );
		else
			Log::Info( "Last Request " \
				+ SUtils::longToString( r->getId() ) \
				+ " in " \
				+ SUtils::longToString( pos ) \
				+ " pos, is not ReadyToSend" );
	}
	else
		Log::Info( "No Requests on checkPendingToSend" );
	return ( 0 );
}

Request*	Requests::getPending( void )
{
	if ( this->size() > 0 )
		return (this->front());
	return ( NULL );
}
