/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:53:27 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/17 12:26:43 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUESTS_HPP_
# define _REQUESTS_HPP_
# include <poll.h>
# include <map>
# include "Request.hpp"

class Requests : public std::map<struct pollfd*, Request*>
{
private:
public:
	Requests();
	~Requests();
	Requests(const Requests& b);
	Requests& operator=(const Requests& b);
	Request* appendRequest(struct pollfd*);
	int	eraseRequest(Request* req);
	int	eraseRequest(struct pollfd* clientPoll);
};

#endif
