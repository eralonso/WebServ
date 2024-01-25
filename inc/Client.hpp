/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/25 11:33:06 by omoreno-         ###   ########.fr       */
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
# include <Response.hpp>
# include <Request.hpp>
# include <CgisMap.hpp>
# include <TypesDefines.hpp>

class Client: public Requests
{
private:
	static size_t		id_counter;
	size_t				id;
	socket_t			socket;
	size_t				pending;
	std::string			received;
	bool				keepAlive;
	WSPoll				*polls;
	struct sockaddr_in	addr;
	Response			*res;
public:
	ServersVector	*servers;
	// CgisMap	cgis;
public:
	Client( void );
	Client( socket_t pollsocket, WSPoll& polls, \
				ServersVector& servers, struct sockaddr_in& info );
	virtual ~Client( void );
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
	int			sendResponse( Response *res );
	bool		getLine( std::string& line );
	size_t		getNChars( std::string& data, size_t n );
	size_t		getPendingSize( void ) const;
	// int 		setDummyRecv();
	bool		setKeepAlive( bool value );
	size_t		purgeUsedRecv( void );
	void		allowPollWrite( bool value );
	bool		checkPendingToSend( void );
	const ServersVector&	getServers( void ) const;
	const struct sockaddr_in&	getAddr( void ) const;
	std::string					getIpString( void ) const;
	unsigned int				getIpNetworkOrder( void ) const;
	unsigned int				getIpHostOrder( void ) const;
	bool		isResponsePendingToSend( void ) const;
};

#endif
