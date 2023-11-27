/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 14:14:58 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <poll.h>
# include <unistd.h>
# include <Sockets.hpp>
# include <WSPoll.hpp>
# include <WSSignals.hpp>
# include <Requests.hpp>

class Client : public Requests
{
public:
private:
	struct pollfd*			clientPoll;
public:
	Client(void);
	Client(struct pollfd*	cientPoll);
	~Client();
	struct pollfd*			getClientPoll();
	Request* 				findRecvRequest();
	Request* 				findCompleteRecvRequest();
	Request*				findReadyToSendRequest();
	int						manageRecv(std::string recv);
	int						managePollout();
	std::string				getHtml( void );
	std::string				getResponse(Request *req);
	int						sendResponse(std::string resp);
};

#endif