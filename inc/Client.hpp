/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/12 13:24:27 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <unistd.h>

# include <string>

# include <Sockets.hpp>
# include <WSSignals.hpp>
# include <Requests.hpp>
# include <Response.hpp>
# include <Request.hpp>
# include <CgisMap.hpp>
# include <TypesDefines.hpp>
# include <EventsTarget.hpp>

class Client: public Requests, public EventsTarget
{
private:
	static size_t		id_counter;
	size_t				id;
	size_t				pending;
	std::string			received;
	std::string			cgiOutput;
	bool				keepAlive;
	struct sockaddr_in	addr;
	Response			*res;
	Receptionist		*receptionist;
	socket_t			socket;
	int					fileFd;
	int					pipeCgiWrite;
	int					pipeCgiRead;
	size_t				requestBodyRemain;
	size_t				responseBodyRemain;
	bool				cgiDriven;
	bool				cgiFinished;
	bool				cgiTimeout;
	bool				responseHeaderSent;
	bool				responseSent;
	bool				readEOF;
	bool				writeEOF;
public:
	typedef enum e_sendStatus
	{
		ERROR,
		SENDING,
		SENT
	} t_sendStatus;
	const ServersVector	*servers;
public:
	Client( void );
	Client( socket_t socket, Events *bEvs, \
				const ServersVector *servers, struct sockaddr_in& info, \
				Receptionist *recp );
	virtual ~Client( void );
	Client( const Client& b );
	Client&	operator=( const Client& b );
public:
	int							bindClientPoll( socket_t socket );
	bool						isResponseSent( void ) const;
	socket_t					getClientSocket( void ) const;
	size_t						getId( void ) const;
	void						LogId( void ) const;
	Request*					findRecvRequest( void );
	Request*					findCompleteRecvRequest( void );
	Request*					findReadyToSendRequest( void );
	int							manageRecv( std::string recv );
	bool						getKeepAlive( void ) const;
	bool						getLine( std::string& line );
	size_t						getNChars( std::string& data, size_t n );
	size_t						getPendingSize( void ) const;
	bool						setKeepAlive( bool value );
	size_t						purgeUsedRecv( void );
	bool						checkPendingToSend( void );
	Response					*createResponse( void );
	const ServersVector&		getServers( void ) const;
	const struct sockaddr_in&	getAddr( void ) const;
	std::string					getIpString( void ) const;
	unsigned int				getIpNetworkOrder( void ) const;
	unsigned int				getIpHostOrder( void ) const;
	int							getFileFd( void ) const;
	int							getPipeCgiWrite( void ) const;
	int							getPipeCgiRead( void ) const;
	socket_t					getSocket( void ) const;
	const std::string&			getCgiOutput( void ) const;
	bool						isResponsePendingToSend( void ) const;
	void						setFileFd( int fd );
	void						setPipeCgiWrite( int fd );
	void						setPipeCgiRead( int fd );
	int							setEventReadSocket( void );
	int							setEventWriteSocket( void );
	int							setEventProc( int pipeRead, int pipeWrite, int pid );
	int							setEventReadFile( int fd );
	int							setEventWriteFile( int fd );
	int							enableEventReadSocket( bool enable );
	int							enableEventWriteSocket( bool enable );
	int							deleteEventProcExit( pid_t pid );
	int							deleteEventProcTimeout( pid_t pid );
	int							onEventProcExit( Event& tevent );
	int							onEventProcTimeout( Event& tevent );
	int							onEventReadSocket( Event& tevent );
	int							onEventReadFile( Event& tevent );
	int							onEventReadPipe( Event& tevent );
	int							onEventWriteSocket( Event& tevent );
	int							onEventWriteFile( Event& tevent );
	int							onEventWritePipe( Event& tevent );
	int							onEventRead( Event& tevent );
	int							onEventWrite( Event& tevent );
	int							onEvent( Event& tevent );
	static int 					readRequest(socket_t clientFd, std::string & readed);
	void						setCompletedRequest( void );
	void						setTimeoutedRequest( void );
	void						performCgiCompletion(void);
	void						resetCgiOperation( void );
	void						nextRequest( void );
	void						reset( void );
};

#endif
