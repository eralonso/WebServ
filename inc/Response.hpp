/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:07 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 11:13:32 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

# include <unistd.h>
# include <sys/socket.h>

# include <string>

# include "Headers.hpp"
# include "StatusCodes.hpp"
# include "Defines.hpp"
# include "Events.hpp"
# include "Log.hpp"

class Response
{
public:
	typedef enum e_sendStatus
	{
		ERROR,
		GETTING_DATA,
		SENDING,
		SENT
	} t_sendStatus;
private:
	std::string		server;
	std::string		protocol;
	std::string		method;
	std::string		route;
	unsigned int	status;
	std::string		query;
	Headers			headers;
	std::string		headersString;
	std::string		body;
	bool			isCgi;
	size_t			sendPos;
	std::string		resString;
	t_sendStatus	sendState;
public:
	Response( void );
	~Response( void );
	Response( const Response& b );
	Response&		operator=( const Response& b );
	void	 		setServer( std::string server );
	void	 		setProtocol( std::string protocol );
	void	 		setMethod( std::string method );
	void	 		setRoute( std::string route );
	void	 		setStatus( unsigned int status );
	void	 		setQuery( std::string query );
	void	 		setHeaders( Headers headers );
	void	 		setBody( std::string content );
	void			setBodyLength( size_t len );
	void			setIsCgi(bool value);
	size_t			increaseSendPos( size_t value );
	void	 		appendHeader( Header header );
	void	 		replaceHeader( Header header );
	std::string		getServer( void ) const;
	std::string		getProtocol( void ) const;
	std::string		getMethod( void ) const;
	std::string		getRoute( void ) const;
	unsigned int	getStatus( void ) const;
	std::string		getResult( void ) const;
	std::string		getQuery( void ) const;
	const Headers&	getHeaders( void ) const;
	Header*			getHeader( std::string key );
	size_t			getContentLength( void ) const;
	std::string		getBody( void ) const;
	bool			getIsCgi( void ) const;
	size_t			getSendPos( void ) const;
	const std::string&	getResString( void ) const;
	t_sendStatus	getSendStatus( void ) const;
	std::string		toString( void ) const;
	void			updateHeadersString( void );
	void			updateResString( void );
	int				sendResponse( Event& tevent );
};

#endif
