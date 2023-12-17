/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 14:37:05 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_

# include <string>
# include <vector>

# include <Headers.hpp>
# include <Defines.hpp>

class Client;

class Request
{
public:
	typedef enum e_status
	{
		IDLE,
		FD_BOND,
		RECVD_START,
		RECVD_REQ_LINE,
		RECVD_HEADER,
		RECVD_CHUNK_SIZE,
		RECVD_CHUNK,
		RECVD_LAST_CHUNK,
		RECVD_ALL,
		CGI_LAUNCHED,
		RESP_RENDERED
	}	t_status;
private:
	static size_t	id_counter;
	size_t			id;
	t_status		status;
	int				error;
	std::string		cgiOutput;
	bool			useCgi;
	size_t 			chunkSize;
	Client			*client;
	std::string		protocol;
	std::string		method;
	std::string		route;
	std::string		query;
	Headers			headers;
	std::string		body;
	bool			badRequest;
	StringVector	routeChain;
	std::string		document;
	std::string		docExt;
private:
	void	parseRoute( void );
	void	parseFirstLine( const std::string &line );
	void	parseHeader( const std::string &line );
	bool	processLineOnFdBond( const std::string &line );
	bool	processLineOnRecvdStart( const std::string &line );
	bool	processLineOnRecvdReqLine( const std::string &line );
	bool	processLineOnRecvdHeader( const std::string &line );
	bool	processLineOnRecvdChunkSize( const std::string &line );
	bool	processLineOnRecvdChunk( const std::string &line );
	bool	processLineOnRecvdLastChunk( const std::string &line );
	bool	checkChunked( void );
	bool	checkKeepAlive( void );
	int		splitDocExt( void );
	bool	checkEmptyContent( size_t& size );
public:
	Request( void );
	Request( Client *client );
	~Request( void );
	Request( const Request& b );
	Request&	operator=( const Request& b );
public:
	int				bindClient( Client* cli );
	t_status		getStatus( void ) const;
	int				getError( void ) const;
	std::string		getCgiOutput( void ) const;
	bool			getUseCgi( void ) const;
	Client*			getClient( void ) const;
	std::string		getProtocol( void ) const;
	std::string		getMethod( void ) const;
	std::string		getRoute( void ) const;
	StringVector	getRouteChaine( void ) const;
	std::string		getRouteChaineString( void ) const;
	std::string		getDocument( void ) const;
	std::string		getDocExt( void ) const;
	std::string		getQuery( void ) const;
	const Headers&	getHeaders( void ) const;
	std::string		getHost( void );
	size_t			getBodyLength( void ) const;
	std::string		getBody( void ) const;
	size_t			getId( void ) const;
	bool			isCompleteRecv( void ) const;
	bool			isReadyToSend( void ) const;
	bool			isCgiLaunched( void ) const;
	bool			isReceiving( void ) const;
	std::string		toString( void );
	void			setBody( const std::string& content );
	void			setReadyToSend( void );
	void			setCgiLaunched( void );
	void			setCgiOutput( std::string str );
	void			setUseCgi( bool value );
	void			setError( int err );
	void			logStatus( void );
	bool			processLine( const std::string& line );
};

#endif
