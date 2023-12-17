/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:58:31 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 13:05:20 by eralonso         ###   ########.fr       */
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

Requests::Requests( const Requests& b ): std::vector< Request* >( b ) {}

Requests&	Requests::operator=( const Requests& b )
{
	if ( this == &b )
		std::vector< Request* >::operator=( b );
	return ( *this );
}

Request	*Requests::appendRequest( Client *cli )
{
	Request	*req = new Request( cli );

	if ( !req )
		return ( NULL );
	this->push_back( req );
	return ( req );
}

int	Requests::eraseRequest( void )
{
	Requests::iterator	ite;
	Request				*req = NULL;

	if ( this->size() > 0 )
	{
		ite = this->end() - 1;
		req = *ite;
		if ( req != NULL )
		{
			this->erase( ite );
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
		r = this->operator[]( pos - 1 );
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
