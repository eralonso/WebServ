/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/23 16:04:51 by omoreno-         ###   ########.fr       */
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
		FD_BOND,
		RECVD_START,
		RECVD_REQ_LINE,
		RECVD_HEADER,
		RECVD_CHUNK_SIZE,
		RECVD_CHUNK,
		RECVD_LAST_CHUNK,
		RECVD_ALL,
		DECODED,
		RESP_RENDERED
	}	t_status;
private:
	t_status					status;
	struct pollfd				*clientPoll;
	size_t						pending;
	size_t 						headerSize;
	size_t 						bodySize;
	size_t 						chunkSize;
	std::string					received;
	std::string					protocol;
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
	void 								parseRoute(void);
	void 								parseFirstLine(const std::string &line);
	void 								parseHeader(const std::string &line);
	void 								parseHead(const std::string &head);
	bool								checkHeaderCompleteRecv();
	bool								checkCompleteRecv();
	bool								getLine(std::string& line);
	bool								processLineOnFdBond(const std::string &line);
	bool								processLineOnRecvdStart(const std::string &line);
	bool								processLineOnRecvdReqLine(const std::string &line);
	bool								processLineOnRecvdHeader(const std::string &line);
	bool								processLineOnRecvdChunkSize(const std::string &line);
	bool								processLineOnRecvdChunk(const std::string &line);
	bool								processLineOnRecvdLastChunk(const std::string &line);
	bool								processLine(const std::string& line);
public:
	Request(void);
	Request(struct pollfd *clientPoll);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	int bindClient(struct pollfd *clientPoll);
	int appendRecv(const std::string &recv);
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
	bool								isDecoded() const;
	std::string							toString();
	void								setBody(const std::string& content);
	int									setDummyRecv();
	
};

#endif
