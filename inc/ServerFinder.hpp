/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFinder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:58:06 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/20 18:24:43 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVERFINDER_HPP_
# define _SERVERFINDER_HPP_

# include <TypesDefines.hpp>
# include <Server.hpp>

class ServerFinder
{
private:
	ServerFinder( void );
	~ServerFinder();
public:
	static Server* find(ServersVector servers, std::string host, std::string port);

};

#endif