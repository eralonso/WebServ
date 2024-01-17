/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:28:35 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/17 16:06:09 by codespace        ###   ########.fr       */
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
    static bool	processHeadRequest(Request &req);
    static bool processPostRequest(Request &req);
    static bool	processPutRequest(Request &req);
    static bool processDeleteRequest(Request &req);
private:
	Router( void );
	~Router();
	Router( const Router& );
	Router&	operator=( const Router& );
public:
	static int updateResponse(Response& res, Request& req);
	static Response* getResponse(Request *req);
	static std::string getRequestEmbed(Request& req);
	static Response* formatErrorResponse(Response& res, int error);
	static Response* formatGenericResponse(Response& res, Request& req);
    static bool 	parseCgiHeaderLine(Response &res, Request &req, const std::string &line);
    static bool 	parseCgiHeaders(Response &res, Request &req, const std::string &cgiOut);
    static bool 	parseCgiOutput(Response &res, Request &req);
    static Response *formatCgiResponse(Response &res, Request &req);
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
	static bool			processDirectory( Request& req, std::string path, \
							std::string& output );
	static std::string	getDefaultErrorPage( unsigned int code );
	static int			getFileToRead( Request& req, std::string& file );
	static void			checkRedir( Request& req );
	static void 		checkErrorRedir( int errorStatus, Request& req );
	static void			checkErrorBody( Request& req, int errorStatus );
};

#endif
