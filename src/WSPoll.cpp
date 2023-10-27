/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSPoll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:05:51 by eralonso          #+#    #+#             */
/*   Updated: 2023/10/27 18:26:06 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WSPoll.hpp"

//OCCF = Orthodox canonical class form

//OCCF: Default constructor
WSPoll::WSPoll( void ) {}

//Constructor with max size as parameter
WSPoll::WSPoll( unsigned int maxSize ): _maxSize( maxSize ), \
										_size( 0 ), \
										_serverSizeFd( 0 )
{
	this->_polls = new struct pollfd[ maxSize ];
	restartPoll( 0, this->_maxSize );
}

//OCCF: Copy constructor
WSPoll::WSPoll( const WSPoll& wspoll ): _maxSize( wspoll._maxSize ), \
										_size( wspoll._size ), \
										_serverSizeFd( wspoll._serverSizeFd )
{
	if ( wspoll._polls != NULL )
	{
		this->deletePolls();
		this->_polls = new struct pollfd[ this->_maxSize ];
		for ( unsigned int i = 0; i < this->_maxSize; i++ )
			this->_polls[ i ] = wspoll._polls[ i ];
	}
}

//OCCF: Default destructor
WSPoll::~WSPoll( void )
{
	if ( this->_polls != NULL )
	{
		closePoll( 0, this->_size );
		delete this->_polls;
	}
}

//OCCF: Assignment operator
WSPoll	WSPoll::operator=( const WSPoll& wspoll )
{
	if ( wspoll._polls != NULL )
	{
		this->deletePolls();
		this->_maxSize = wspoll._maxSize;
		this->_size = wspoll._size;
		this->_polls = new struct pollfd[ this->_maxSize ];
		for ( unsigned int i = 0; i < this->_maxSize; i++ )
			this->_polls[ i ] = wspoll._polls[ i ];
	}
	return ( *this );
}

//Getter: Get maxSize
unsigned int	WSPoll::getMaxSize( void ) const
{
	return ( this->_maxSize );
}

//Getter: Get size
unsigned int	WSPoll::getSize( void ) const
{
	return ( this->_size );
}

//Getter: Get size of fd that are from server
unsigned int	WSPoll::getServerSizeFd( void ) const
{
	return ( this->_serverSizeFd );
}

//Getter: Get constant reference of pollfd in 'pos' index
const struct pollfd&	WSPoll::operator[]( unsigned int pos ) const
{
	if ( pos >= this->_maxSize )
		throw std::out_of_range( "Index out of range [ 0 - " \
								+ SUtils::LongToString( this->_maxSize - 1 ) \
								+ " ]" );
	return ( this->_polls[ pos ] );
}

//Getter: Get reference of pollfd in 'pos' index
struct pollfd&	WSPoll::operator[]( unsigned int pos )
{
	if ( pos >= this->_maxSize )
		throw std::out_of_range( "Index out of range [ 0 - " \
								+ SUtils::LongToString( this->_maxSize - 1 ) \
								+ " ]" );
	return ( this->_polls[ pos ] );
}

//Delete array of pollfd's if it's not NULL
void	WSPoll::deletePolls( void )
{
	if ( this->_polls != NULL )
	{
		delete this->_polls;
		this->_polls = NULL;
	}
}

//Add new pollfd to array
bool	WSPoll::addPollfd( socket_t fd, int events, int revents, int type )
{
	if ( this->_size >= this->_maxSize )
		return ( false );
	this->_polls[ this->_size ].fd = fd;
	this->_polls[ this->_size ].events = events;
	this->_polls[ this->_size ].revents = revents;
	this->_size++;
	if ( type == SPOLLFD )
		this->_serverSizeFd++;
	return ( true );
}

//Check return of poll() function
int	WSPoll::checkPollReturn( int ret ) const
{
	if ( ret < 0 )
		std::cerr << "Error: Poll" << std::endl;
	else if ( ret == 0 )
		std::cerr << "Error: Poll timeout" << std::endl;
	return ( ret );
}

//Restart value of pollfd in 'pos' index
void	WSPoll::restartPoll( unsigned int pos )
{
	if ( pos >= this->_maxSize )
		return ;
	this->_polls[ pos ].fd = 0;
	this->_polls[ pos ].events = 0;
	this->_polls[ pos ].revents = 0;
}

//Restart value of pollfds in range start-end index
void	WSPoll::restartPoll( unsigned int start, unsigned int end )
{
	for ( unsigned int i = start; i < end && i < this->_maxSize; i++ )
	{
		this->_polls[ i ].fd = 0;
		this->_polls[ i ].events = 0;
		this->_polls[ i ].revents = 0;
	}
}

//Close fd of pollfd in 'pos' index and restart it value
void	WSPoll::closePoll( unsigned int pos )
{
	if ( pos >= this->_maxSize )
		return ;
	std::cout << "Log: Connection closed with pos [ " << this->_polls[ pos ].fd << " ]" << std::endl;
	close( this->_polls[ pos ].fd );
	restartPoll( pos );
	compressPolls( pos );
	this->_size--;
}

//Close fd of pollfds in range start-end index and restart it value
void	WSPoll::closePoll( unsigned int start, unsigned int end )
{
	for ( unsigned int i = start; i < end && i < this->_maxSize; i++ )
	{
		std::cout << "Log: Connection closed with range [ " << this->_polls[ i ].fd << " ]" << std::endl;
		close( this->_polls[ i ].fd );
		restartPoll( i );
		this->_size--;
	}
	compressPolls( start );
}

//Close fd of pollfd that coincide with the fd passed as parameter and restart it value
void	WSPoll::closePoll( socket_t fd )
{
	for ( unsigned int i = 0; i < this->_size; i++ )
	{
		if ( this->_polls[ i ].fd == fd )
		{
			close( this->_polls[ i ].fd );
			std::cout << "Log: Connection closed with socket_t [ " << fd << " ]" << std::endl;
			restartPoll( i );
			this->_size--;
			compressPolls( i );
			break ;
		}
	}
}

//Compress pollfd array after closing poll
void	WSPoll::compressPolls( unsigned int start )
{
	for ( unsigned int i = start; i < this->_maxSize - 1; i++ )
		this->_polls[ i ] = this->_polls[ i + 1 ];
	restartPoll( this->_maxSize - 1 );
}

//Wait a fd to become ready to perform I/O
int	WSPoll::wait( int timeout )
{
	if ( checkPollReturn( poll( this->_polls, this->_size, timeout ) ) <= 0 )
		return ( -1 );
	return ( 0 );
}

//Return if fd ready to perform I/O is from server or client
socket_t	WSPoll::isNewClient( void )
{
	for ( unsigned int i = 0; i < this->_serverSizeFd; i++ )
	{
		if ( this->_polls[ i ].revents & POLLIN )
			return ( this->_polls[ i ].fd );
	}
	return ( 0 );
}

//Return client fd ready to perform I/O
socket_t	WSPoll::getPerformClient( void )
{
	for ( unsigned int i = this->_serverSizeFd; i < this->_size; i++ )
	{
		if ( this->_polls[ i ].revents & POLLIN )
			return ( this->_polls[ i ].fd );
	}
	return ( 0 );
}
