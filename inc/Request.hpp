/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/15 13:10:07 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <vector>
# include <map>
# include <Headers.hpp>

class Request
{
private:
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
public:
	Request(const std::string& received);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	void								parse(const std::string& received);
	std::string							getMethod() const;
	std::string							getRoute() const;
	std::string							getQuery() const;
	const Headers&						getHeaders() const;
	size_t								getContentLenght() const;
	std::string							getBody() const;
	std::string							toString();
	void								setBody(const std::string& content);
};

#endif
