/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 19:38:34 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <vector>
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
		CGI_LAUNCHED,
		RESP_RENDERED
	}	t_status;
private:
	t_status					status;
	int							error;
	std::string					cgiOutput;
	bool						useCgi;
	size_t 						chunkSize;
	Client*						client;
	std::string					protocol;
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
	bool						badRequest;
	std::vector<std::string>	routeChain;
	std::string					document;
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
	int									getError() const;
	std::string							getCgiOutput() const;
	bool								getUseCgi() const;
	Client*								getClient() ;
	std::string							getProtocol() const;
	std::string							getMethod() const;
	std::string							getRoute() const;
	std::vector<std::string>			getRouteChaine() const;
	std::string							getRouteChaineString() const;
	std::string							getDocument() const;
	std::string							getQuery() const;
	const Headers&						getHeaders() const;
	std::string							getHost();
	size_t								getBodyLength() const;
	std::string							getBody() const;
	bool								isCompleteRecv() const;
	bool								isReadyToSend() const;
	bool								isCgiLaunched() const;
	bool								isReceiving() const;
	std::string							toString();
	void								setBody(const std::string& content);
	void								setReadyToSend();
	void								setCgiLaunched();
	void								setCgiOutput(std::string str);
	void								setError(int);
	void								logStatus();
	bool								processLine(const std::string& line);
};

#endif
