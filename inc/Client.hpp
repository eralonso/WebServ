/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:42:33 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/30 19:20:41 by omoreno-         ###   ########.fr       */
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
	bool					keepAlive;
public:
	Client(void);
	Client(struct pollfd* cliPoll);
	~Client();
	Client(const Client& b);
	Client&	operator=(const Client& b);
	int bindClientPoll(struct pollfd* cliPoll);
	struct pollfd*			getClientPoll();
	Request* 				findRecvRequest();
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
};

#endif