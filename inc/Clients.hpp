/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:36 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 12:07:14 by omoreno-         ###   ########.fr       */
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
	Client*	newClient(struct pollfd* poll);
	int		eraseClient(Client* cli);
	int		eraseClient(struct pollfd* poll);
};


#endif
