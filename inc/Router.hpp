/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:35 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 11:06:38 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ROUTER_HPP_
# define _ROUTER_HPP_
# include <Request.hpp>
# include <Response.hpp>

class Router
{
private:
public:
	Router(/* args */);
	~Router();
	static int updateResponse(Response& res, Request& req);
	static std::string getHtml(Request *req);
	static std::string getForm(void);
	static Response* getResponse(Request *req);
	static Response* createFaviconRes(Response& res, Request& req);
	static std::string getRequestEmbed(Request& req);
	static Response* formatErrorResponse(Response& res, int error);
	static Response* formatGenericResponse(Response& res, Request& req);
	static bool processRequestReceived(Request& req);
};

#endif
