/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:49:07 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:23:42 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_
# include <string>
# include <vector>
# include <map>

class Response
{
private:
	std::string					server;
	std::string					protocol;
	std::string					method;
	std::string					route;
	unsigned int				status;
	std::string					query;
	std::vector<std::string>	headers;
	std::vector<std::string>	contentTypes;
	size_t						contentLength;
	std::string					body;
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
	void	 					setHeaders(std::vector<std::string> headers);
	void	 					setContentTypes(std::vector<std::string> contentTypes);
	void						setContentLength(size_t size);
	void	 					setBody(std::string body);
	void	 					pushHeader(std::string header);
	void	 					pushContentType(std::string contentType);
	const std::string				getServer() const;
	const std::string				getProtocol() const;
	const std::string				getMethod() const;
	const std::string				getRoute() const;
	unsigned int					getStatus() const;
	const std::string				getResult() const;
	const std::string				getQuery() const;
	const std::vector<std::string>	getHeaders() const;
	const std::vector<std::string>	getContentTypes() const;
	const size_t					getContentLength() const;
	const std::string				getBody() const;
	const std::string				contentTypesToString() const;
	const std::string				headersToString() const;
	const std::string				toString() const;
};

#endif