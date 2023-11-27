/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:53:27 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 15:43:44 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUESTS_HPP_
# define _REQUESTS_HPP_
# include <poll.h>
# include <vector>
# include <Request.hpp>

class Requests : public std::vector<Request*>
{
private:
public:
	Requests();
	~Requests();
	Requests(const Requests& b);
	Requests& operator=(const Requests& b);
	Request* appendRequest(Client*);
	int	eraseRequest();
};

#endif
