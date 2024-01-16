/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/16 11:33:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_

# include <string>
# include <vector>

# include <Headers.hpp>
# include <Defines.hpp>
# include <DefinesFunctions.hpp>
# include <Server.hpp>
# include <ConfigUtils.hpp>

class Client;

class Request
{
public:
	typedef enum e_status
	{
		IDLE,
		FD_BOND,
		RECVD_START,
		RECVD_REQ_LINE,
		RECVD_HEADER,
		RECVD_CHUNK_SIZE,
		RECVD_CHUNK,
		RECVD_LAST_CHUNK,
		RECVD_ALL,
		CGI_LAUNCHED,
		RESP_RENDERED
	}	t_status;
private:
	static size_t	id_counter;
	size_t			id;
	t_status		status;
	int				error;
	bool			useCgi;
	bool			badRequest;
	bool			redir;
	size_t 			chunkSize;
	size_t			maxBodySize;
	Client			*client;
	std::string		cgiOutput;
	std::string		output;
	std::string		method;
	std::string		url;
	std::string		route;
	std::string		routeHost;
	std::string		routePort;
	std::string		query;
	std::string		filePath;
	std::string		protocol;
	std::string		body;
	std::string		document;
	std::string		docExt;
	std::string		uriRedir;
	StringVector	routeChain;
	Headers			headers;
	const Server	*svr;
	const Location	*lc;
	size_t			outputLength;
private:
	void 	parseHostPortFromRoute( void );
	void	parseQueryStringFromRoute( void );
    bool 	parseDropHttp(StringVector &tokens, size_t &tokensSize, bool &httpDropped);
    void 	parseRoute(void);
    void	parseFirstLine( const std::string &line );
	void	parseHeader( const std::string &line );
	bool	processLineOnFdBond( const std::string &line );
	bool	processLineOnRecvdStart( const std::string &line );
	bool	processLineOnRecvdReqLine( const std::string &line );
	bool	processLineOnRecvdHeader( const std::string &line );
	bool	processLineOnRecvdChunkSize( const std::string &line );
	bool	processLineOnRecvdChunk( const std::string &line );
	bool	processLineOnRecvdLastChunk( const std::string &line );
	bool	checkProtocol( std::string protocol );
	bool	checkChunked( void );
	bool	checkKeepAlive( void );
	int		splitDocExt( void );
	bool	checkEmptyContent( size_t& size );
	bool	updateServerConfig( void );
	void	updateLocation( void );
	void	updateFilePath( void );

public:
    Request( void );
	Request( Client *client );
	~Request( void );
	Request( const Request& b );
	Request&	operator=( const Request& b );
public:
	int				bindClient( Client* cli );
	t_status 		getStatus(void) const;
	int				getError( void ) const;
	std::string		getCgiOutput( void ) const;
	void 			checkUseCgi(void);
	bool 			getUseCgi(void) const;
	Client*			getClient( void ) const;
	std::string		getProtocol( void ) const;
	std::string		getMethod( void ) const;
	std::string		getRoute( void ) const;
	StringVector	getRouteChaine( void ) const;
	std::string		getRouteChaineString( void ) const;
	std::string		getDocument( void ) const;
	bool			isDocumentNPH( ) const;
	std::string		getDocExt( void ) const;
	std::string		getQuery( void ) const;
	const Headers&	getHeaders( void ) const;
	std::string		getHost( void ) const;
	std::string 	getPort( void ) const;
	bool 			getHostPort(std::string &host, std::string &port) const;
	std::string		getHeaderWithKey(const std::string &key);
	size_t 			getBodyLength( void ) const;
	std::string		getBody( void ) const;
	size_t			getId( void ) const;
	std::string		getFilePath( void ) const;
	const Server	*getServer( void ) const;
	const Location	*getLocation( void ) const;
	std::string		getFinalPath( void ) const;
	std::string		getOutput( void ) const;
	bool			getErrorPage( int error, std::string& uriRedir );
	bool			getRedir( void ) const;
	std::string 	getCgiBinary( std::string ext ) const;
	std::string		getUriRedir( void ) const;
	size_t			getOutputLength( void ) const;
    bool			isDirectiveSet( std::string directive ) const;
    bool			isCompleteRecv( void ) const;
	bool			isReadyToSend( void ) const;
	bool			isCgiLaunched( void ) const;
	bool			isReceiving( void ) const;
	bool			isAutoindexAllow( void ) const;
	std::string		toString( void );
	void			setBody( const std::string& content );
	void			setReadyToSend( void );
	void			setReceivedAll();
	void			setCgiLaunched( void );
	void			setCgiOutput( std::string str );
	void			setUseCgi( bool value );
	void			setOutput( std::string str );
	bool			setError( int err );
	void			setRedir( bool isRedir );
	void			setUriRedir( std::string uriRedirection );
	void			setRedirection( std::string uri, int code );
	void			setOutputLength( size_t size );
	void			logStatus( void );
	bool			processLine( const std::string& line );
	bool			tryIndexFiles( std::string& file ) const;
	void			setDefaultFavicon( void );
	bool			findReturnUri( int& uriCode, std::string& uriRedir ) const;
};

#endif
