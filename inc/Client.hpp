/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 18:06:12 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <poll.h>
# include <unistd.h>
# include <string>
# include <Sockets.hpp>
# include <WSPoll.hpp>
# include <WSSignals.hpp>
# include <Requests.hpp>
# include <Request.hpp>

class Client : public Requests
{
public:
private:
	struct pollfd*			clientPoll;
	size_t					pending;
	std::string				received;
public:
	Client(void);
	Client(struct pollfd*	cientPoll);
	~Client();
	struct pollfd*			getClientPoll();
	Request* 				findRecvRequest();
	Request* 				findCompleteRecvRequest();
	Request*				findReadyToSendRequest();
	int						manageRecv(std::string recv);
	int						manageCompleteRecv();
	int						managePollout();
	std::string				getHtml( void );
	std::string				getResponse(Request *req);
	int						sendResponse(std::string resp);
	bool					getLine(std::string& line);
	size_t					getPendingSize() const;
	int 					setDummyRecv();
	size_t					purgeUsedRecv();
};

#endif