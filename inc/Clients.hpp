/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/28 10:29:44 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENTS_HPP_
# define _CLIENTS_HPP_

# include <Client.hpp>
# include <map>

class Clients : public std::map<struct pollfd*, Client*>
{
private:
public:
	Clients(/* args */);
	~Clients();
	Clients(const Clients& b);
	Clients& operator=(const Clients& b);
	Client*	newClient(struct pollfd* poll);
	int		eraseClient(Client* cli);
	int		eraseClient(struct pollfd* poll);
};


#endif
