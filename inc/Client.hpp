/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/19 18:30:56 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <poll.h>
# include <unistd.h>
# include <string>
# include <Sockets.hpp>
# include <WSPoll.hpp>
# include <WSSignals.hpp>
# include <Requests.hpp>
# include <Request.hpp>
# include <CgisMap.hpp>

class Client: public Requests
{
private:
	static size_t	id_counter;
	size_t		 	id;
	socket_t	 	socket;
	size_t		 	pending;
	std::string	 	received;
	bool		 	keepAlive;
	WSPoll			*polls;
public:
	CgisMap	cgis;
public:
	Client( void );
	Client( socket_t pollsocket, WSPoll& polls );
	~Client( void );
	Client( const Client& b );
	Client&	operator=( const Client& b );
public:
	int			bindClientPoll( socket_t pollsocket );
	socket_t	getClientSocket( void ) const;
	size_t 		getId( void ) const;
	void		LogId( void ) const;
	Request*	findRecvRequest( void );
	Request* 	findCompleteRecvRequest( void );
	Request*	findReadyToSendRequest( void );
	int			manageRecv( std::string recv );
	int			manageCompleteRecv( void );
	int			managePollout( void );
	bool		getKeepAlive( void ) const;
	int			sendResponse( std::string resp );
	bool		getLine( std::string& line );
	size_t		getNChars( std::string& data, size_t n );
	size_t		getPendingSize( void ) const;
	// int 		setDummyRecv();
	bool		setKeepAlive( bool value );
	size_t		purgeUsedRecv( void );
	void		allowPollWrite( bool value );
	bool		checkPendingToSend( void );
};

#endif
