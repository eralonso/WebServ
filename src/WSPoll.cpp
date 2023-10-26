/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSPoll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:05:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/26 19:33:03 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WSPoll.hpp"

WSPoll::WSPoll( void ) {}

WSPoll::WSPoll( unsigned int maxSize ): _maxSize( maxSize ), pos( 0 )
{
	this->_polls = new struct pollfd[ maxSize ];
	restartPolls( 0, this->_maxSize );
}

WSPoll::WSPoll( const WSPoll& wspoll ): _maxSize( wspoll._maxSize ), _pos( wspoll._pos )
{
	if ( wspoll._polls != NULL )
	{
		this->deletePolls();
		this->_polls = new struct pollfd[ this->_maxSize ];
		for ( unsigned int i = 0; i < this->_maxSize; i++ )
			this->_polls[ i ] = wspoll._polls[ i ];
	}
}

WSPoll::~WSPoll( void )
{
	if ( this->_polls != NULL )
		delete this->_polls;
}

WSPoll	WSPoll::operator=( const WSPoll& wspoll )
{
	if ( wspoll._polls != NULL )
	{
		this->deletePolls();
		this->_maxSize = wspoll._maxSize;
		this->_pos = wspoll._pos;
		this->_polls = new struct pollfd[ this->_maxSize ];
		for ( unsigned int i = 0; i < this->_maxSize; i++ )
			this->_polls[ i ] = wspoll._polls[ i ];
	}
	return ( *this );
}

void	WSPoll::deletePolls( void )
{
	if ( this->_polls != NULL )
	{
		delete this->_polls;
		this->_polls = NULL;
	}
}

void	WSPoll::addPollFd( socket_t fd, int events, int revents )
{
	if ( this->_pos >= this->_maxSize )
		return ;
	this->_poll[ this->_pos ].fd = fd;
	this->_poll[ this->_pos ].events = events;
	this->_poll[ this->_pos ].revents = revents;
}

int	WSPoll::checkPollReturn( int ret )
{
	if ( ret < 0 )
		std::cerr << "Error: Poll" << std::endl;
	else if ( ret == 0 )
		std::cerr << "Error: Poll timeout" << std::endl;
	return ( ret );
}

void	WSPoll::restartPoll( unsigned int pos )
{
	if ( pos >= this->_maxSize )
		return ;
	this->_poll[ pos ].fd = 0;
	this->_poll[ pos ].events = 0;
	this->_poll[ pos ].revents = 0;
}

void	WSPoll::restartPoll( unsigned int start, unsigned int end )
{
	for ( unsigned int i = start; i < end && i < this->_maxSize; i++ )
	{
		this->_poll[ i ].fd = 0;
		this->_poll[ i ].events = 0;
		this->_poll[ i ].revents = 0;
	}
}

void	WSPoll::closePoll( unsigned int pos )
{
	if ( pos >= this->_maxSize )
		return ;
	close( this->_polls[ pos ].fd );
	restartPoll( pos );
	this->_pos--;
}

void	WSPoll::closePoll( int start, int end )
{
	for ( unsigned int i = start; i < end && i < this->_maxSize; i++ )
	{
		close( this->_polls[ i ].fd );
		restartPoll( i );
		this->_pos--;
	}
}

void	WSPoll::reallocPolls( unsigned int start )
{
	for ( unsigned int i = start; i < this->_maxSize - 1; i++ )
		this->_polls[ i ] = this->_polls[ i + 1 ];
	restartPoll( this->_maxSize - 1 );
}

