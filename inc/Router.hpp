/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:35 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/08 11:42:58 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ROUTER_HPP_
# define _ROUTER_HPP_

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

# include <Request.hpp>
# include <Response.hpp>
# include <CgiExecutor.hpp>
# include <ActionMask.hpp>

class Router
{
private:
	static std::string	methods[ METHODS_NB ];
	static bool ( *process[ METHODS_NB ] )( Request& req );
	static bool	processGetRequest( Request& req );
	static bool	processPostRequest( Request& req );
	static bool	processDeleteRequest( Request& req );
public:
	Router(/* args */);
	~Router();
	static int updateResponse(Response& res, Request& req);
	static std::string getHtml(Request *req);
	static std::string getHtmlErrorPage(Request *req);
	static std::string getForm(void);
	static Response* getResponse(Request *req);
	static Response* createFaviconRes(Response& res, Request& req);
	static std::string getRequestEmbed(Request& req);
	static Response* formatErrorResponse(Response& res, int error);
	static Response* formatGenericResponse(Response& res, Request& req);
	static Response* formatCgiResponse(Response& res, Request& req);
	static Response* formatContinueResponse(Response &res, Request &req);
	static Response* formatAcceptResponse(Response &res, Request &req);
	static Response* formatErrorResponse(Response &res, Request &req);
	static bool 		processRequestReceived(Request &req);
	static std::string 	determineContentType(Response &res, Request &req);
	static bool			processCgi( Request& req );
	static bool			checkStatMode( std::string path, unsigned int mode );
	static bool			isDir( std::string path );
	static bool			isFile( std::string path );
	static std::string	readFile( std::string file );
    static bool			writeFile(std::string file, std::string content);
    static bool 		checkPathExist(Request &req, std::string path);
    static bool			checkPathCanRead( Request& req, std::string path );
	static bool			processDirectory( Request& req, std::string path );
	static std::string	getDefaultErrorPage( unsigned int code );
};

#endif
