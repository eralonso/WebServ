/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/14 17:25:41 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <vector>
# include <map>

class Request
{
private:
	std::string							method;
	std::string							route;
	std::string							query;
	std::map<std::string, std::string>	headers;
	std::vector<std::string>			contentTypes;
	size_t								contentLength;
	std::string							body;
public:
	Request(const std::string& received);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	void								parse(const std::string& received);
	const std::string&					getMethod() const;
	const std::string&					getRoute() const;
	const std::string&					getQuery() const;
	const std::map<std::string, std::string>&		getHeaders() const;
	const std::vector<std::string>&		getContentTypes() const;
	size_t								getContentLenght() const;
	const std::string&					getBody() const;
	const std::string					contentTypesToString() const;
	std::string							toString() const;
};

#endif
