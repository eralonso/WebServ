/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:53:27 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/17 12:32:15 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUESTS_HPP_
# define _REQUESTS_HPP_

# include <poll.h>
# include <vector>
# include <Request.hpp>

class Requests: public std::vector< Request* >
{
private:
public:
	Requests( void );
	~Requests( void );
	Requests( const Requests& b );
	Requests&	operator=( const Requests& b );
	Request		*appendRequest( Client *cli );
	int			eraseRequest( void );
	bool		checkPendingToSend( void );
};

#endif
