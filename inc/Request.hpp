/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 13:23:41 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <poll.h>
# include "Headers.hpp"

class Request
{
public:
	typedef enum e_status
	{
		IDLE,
		WAITING_RECV,
		RECV_ALL,
		RECV_DECODED,
		RESP_RENDERED
	}	t_status;
private:
	t_status					status;
	struct pollfd				*clientPoll;
	std::string					received;
	std::string					protocol;
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
	void parseRoute(void);
	void parseFirstLine(const std::string &line);
	void parseHeader(const std::string &line);
public:
	Request(void);
	Request(struct pollfd *clientPoll);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	int bindClient(struct pollfd *clientPoll);
	int appendRecv(const std::string &recv, bool finish);
	void parseHead(const std::string &head);
	int									parse();
	t_status							getStatus() const;
	struct pollfd*						getClientPoll() const;
	std::string							getProtocol() const;
	std::string							getMethod() const;
	std::string							getRoute() const;
	std::string							getQuery() const;
	const Headers&						getHeaders() const;
	size_t								getContentLenght() const;
	std::string							getBody() const;
	bool								isReadyToSend() const;
	std::string							toString();
	void								setBody(const std::string& content);
	int									setDummyRecv();
	
};

#endif
