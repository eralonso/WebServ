/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:07 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/14 13:01:08 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_
# include <string>
# include "Headers.hpp"
# include "StatusCodes.hpp"

class Response
{
private:
	std::string					server;
	std::string					protocol;
	std::string					method;
	std::string					route;
	unsigned int				status;
	std::string					query;
	Headers						headers;
	std::string					body;
	bool						isCgi;
public:
	Response();
	~Response();
	Response(const Response& b);
	Response&					operator=(const Response& b);
	void	 					setServer(std::string server);
	void	 					setProtocol(std::string protocol);
	void	 					setMethod(std::string method);
	void	 					setRoute(std::string route);
	void	 					setStatus(unsigned int status);
	void	 					setQuery(std::string query);
	void	 					setHeaders(Headers headers);
	void	 					setBody(std::string content);
	void						setIsCgi(bool value);
	void	 					appendHeader(Header header);
	std::string					getServer() const;
	std::string					getProtocol() const;
	std::string					getMethod() const;
	std::string					getRoute() const;
	unsigned int				getStatus() const;
	std::string					getResult() const;
	std::string					getQuery() const;
	const Headers&				getHeaders() const;
	Header*						getHeader(std::string key);
	size_t						getContentLength() const;
	std::string					getBody() const;
	bool						getIsCgi() const;
	std::string					toString();
};

#endif