/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/29 11:08:20 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <Headers.hpp>

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
		RESP_RENDERED
	}	t_status;
private:
	t_status					status;
	size_t 						chunkSize;
	Client*						client;
	std::string					protocol;
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
	bool						badRequest;
	void 								parseRoute(void);
	void 								parseFirstLine(const std::string &line);
	void 								parseHeader(const std::string &line);
	bool								processLineOnFdBond(const std::string &line);
	bool								processLineOnRecvdStart(const std::string &line);
	bool								processLineOnRecvdReqLine(const std::string &line);
	bool								processLineOnRecvdHeader(const std::string &line);
	bool								processLineOnRecvdChunkSize(const std::string &line);
	bool								processLineOnRecvdChunk(const std::string &line);
	bool								processLineOnRecvdLastChunk(const std::string &line);
	bool								checkChunked();
	bool								checkKeepAlive();
	bool								checkEmptyContent(size_t& size);
public:
	Request(void);
	Request(Client *client);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	int bindClient(Client* cli);
	t_status							getStatus() const;
	Client*								getClient() const;
	std::string							getProtocol() const;
	std::string							getMethod() const;
	std::string							getRoute() const;
	std::string							getQuery() const;
	const Headers&						getHeaders() const;
	size_t								getBodyLength() const;
	std::string							getBody() const;
	bool								isCompleteRecv() const;
	bool								isReadyToSend() const;
	bool								isReceiving() const;
	std::string							toString();
	void								setBody(const std::string& content);
	void								setReadyToSend();
	void								logStatus();
	bool								processLine(const std::string& line);
};

#endif
