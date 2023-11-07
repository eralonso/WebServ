/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIService.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:14:43 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 12:47:59 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CGISERVICE_HPP_
# define _CGISERVICE_HPP_
# include <string>

class CGIService
{
private:
	std::string		name;
	unsigned int	port;
	std::string		url;
	std::string		extension;
public:
	CGIService();
	CGIService(std::string name, unsigned int port, std::string url,
		std::string extension);
	~CGIService();
	unsigned int	getPort(void);
	std::string		getName(void);
	std::string		getUrl(void);
	std::string		getExtension(void);
};


#endif