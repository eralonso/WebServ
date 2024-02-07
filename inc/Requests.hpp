/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:53:27 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/07 16:12:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUESTS_HPP_
# define _REQUESTS_HPP_

# include <queue>
# include <Request.hpp>

typedef std::queue< Request * >	RequestQueue;

class Requests: public RequestQueue
{
private:
public:
	Requests( void );
	virtual ~Requests( void );
	Requests( const Requests& b );
	Requests&	operator=( const Requests& b );
	Request		*appendRequest( Client *cli );
	int			eraseRequest( void );
	bool		checkPendingToSend( void );
	Request*	getPending( void );
};

#endif
