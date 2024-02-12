/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receptionist.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:44:28 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/12 15:39:53 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Receptionist.hpp>
#include <Response.hpp>

Receptionist::Receptionist( ServersVector& servers ): Clients(), \
													EventsTarget( new Events() ), \
													_servers( servers ), \
													timeout( 1 )
{
	if ( this->evs->isCreate() == false )
		throw std::runtime_error( "Cant't create a kqueue" );
	setupServers();
}

Receptionist::~Receptionist( void )
{
	if ( this->evs )
		delete this->evs;
}

Receptionist::Receptionist( const Receptionist& b ): Clients(), \
													EventsTarget( new Events() ), \
													_servers( b._servers ), \
													timeout( b.timeout ) {}

Receptionist& 	Receptionist::operator=( const Receptionist& b )
{
	if ( this != &b )
	{
		this->_servers = b._servers;
		this->timeout = b.timeout;
	}
	return ( *this );
}

void	Receptionist::setupServers( void )
{
	socket_t				serverFd;
	Directives				*d = NULL;
	ServersVector::iterator	itb = this->_servers.begin();
	ServersVector::iterator	it = itb;
	struct sockaddr_in		info;

	while ( it != this->_servers.end() )
	{
		d = it->getDirectives();
		if ( d != NULL && d->isEmpty() == false )
		{
			if ( serverShareAddr( itb, it, info, serverFd ) == false )
			{
				serverFd = Sockets::createPassiveSocket( d->getHost(), \
								d->getPort(), BACKLOG, info );
			}
			it->setAddr( info );
			it->setSocketFd( serverFd );
			it->setReceptionist( this );
			it->setEvents( this->evs );
			it->setEventRead();
			it++;
		}
		else
			this->_servers.erase( it );
	}
}

bool	Receptionist::serverShareAddr( ServersVector::iterator& begin, \
										ServersVector::iterator& curr, \
										struct sockaddr_in& info, \
	   									socket_t& serverFd )
{
	struct sockaddr_in	addr;
	unsigned int		ip;

	addr = Sockets::codeHostToAddrin( curr->getHost().c_str(), curr->getPort() );
	ip = addr.sin_addr.s_addr;
	for ( ServersVector::iterator it = begin; it != curr; it++ )
	{
		if ( it->getPort() == curr->getPort() && it->getIpNetworkOrder() == ip )
		{
			info = it->getAddr();
			serverFd = it->getSocketFd();
			return ( true );
		}
	}
	return ( false );
}

const ServersVector&	Receptionist::getServers( void ) const
{
	return ( this->_servers );
}

int	Receptionist::onEvent( Event & )
{
	Log::Timeout( Events::resetLine, " Pending Clients [ " + SUtils::longToString( this->size() ) + " ]" );
	Events::resetLine = false;
	return ( 0 );
}

int	Receptionist::mainLoop( void )
{
	this->evs->setEventTimer( this, 0, 500000, false );
	return ( this->evs->loopEvents() );
}
