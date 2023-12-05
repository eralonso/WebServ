/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/05 16:54:34 by omoreno-         ###   ########.fr       */
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
#include <CgisMap.hpp>

class Client : public Requests
{
public:
private:
	static size_t						id_counter;
	size_t								id;
	socket_t							socket;
	size_t								pending;
	std::string							received;
	bool								keepAlive;
	WSPoll*								polls;
public:
	CgisMap								cgis;
	Client(void);
	Client(socket_t pollsocket, WSPoll& polls);
	~Client();
	Client(const Client& b);
	Client&	operator=(const Client& b);
	int bindClientPoll(socket_t pollsocket);
	socket_t				getClientSocket();
	size_t 					getId();
	void					LogId();
	Request*				findRecvRequest();
	Request* 				findCompleteRecvRequest();
	Request*				findReadyToSendRequest();
	int						manageRecv(std::string recv);
	int						manageCompleteRecv();
	int						managePollout();
	bool					getKeepAlive() const;
	int						sendResponse(std::string resp);
	bool					getLine(std::string& line);
	size_t					getNChars(std::string& data, size_t n);
	size_t					getPendingSize() const;
	// int 					setDummyRecv();
	bool					setKeepAlive(bool value);
	size_t					purgeUsedRecv();
	void					allowPollWrite(bool value);
	bool					checkPendingToSend();
};

#endif