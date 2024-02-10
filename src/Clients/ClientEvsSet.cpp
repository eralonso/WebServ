/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientEvsSet.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:05:53 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/10 11:24:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CgiExecutor.hpp"
#include <Utils.hpp>

int	Client::setEventReadSocket( void )
{
	Log::Debug("setEventReadSocket [" + SUtils::longToString(this->socket) + "]");
	if ( this->evs )
		return ( this->evs->setEventRead( this, this->socket ) );
	return ( 0 );
}
int	Client::setEventWriteSocket( void )
{
	Log::Debug("setEventWriteSocket [" + SUtils::longToString(this->socket) + "]");
	if ( this->evs )
		return ( this->evs->setEventWrite( this, this->socket ) );
	return ( 0 );
}

int	Client::setEventProc( int pipeRead, int pipeWrite, int pid )
{
	Log::Debug("setEventProc [" + SUtils::longToString(pid) + "]");
	this->pipeCgiRead = pipeRead;
	this->pipeCgiWrite = pipeWrite;
	this->cgiDriven = true;
	if ( this->evs )
	{
		if ( this->evs->setEventProcExit( this, pid, CGI_TO ) )
			return ( -1 );
		if ( this->evs->setEventRead( this, pipeRead ) )
			return ( -1 );
		if ( this->evs->setEventWrite( this, pipeWrite ) )
			return ( -1 );
	}
	return ( 0 );
}

int	Client::setEventReadFile( int fd )
{
	Log::Debug("setEventReadFile [" + SUtils::longToString(fd) + "]");
	this->writeEOF = true;
	this->fileFd = fd;
	if ( this->evs )
		return ( this->evs->setEventRead( this, fd ) );
	return ( 0 );
}

int	Client::setEventWriteFile( int fd )
{
	Log::Debug("setEventWriteFile [" + SUtils::longToString(fd) + "]");
	this->readEOF = true;
	this->fileFd = fd;
	if ( this->evs )
		return ( this->evs->setEventWrite( this, fd ) );
	return ( 0 );
}

int	Client::enableEventReadSocket( bool enable )
{
	Log::Debug(std::string("enableEventReadSocket [" + SUtils::longToString(this->socket) + "] ") + (enable ? "enable" : "disable"));
	if ( this->evs )
		return ( this->evs->enableEventRead( this, this->socket, enable ) );
	return ( 0 );
}

int	Client::enableEventWriteSocket( bool enable )
{
	Log::Debug(std::string("enableEventWriteSocket  [" + SUtils::longToString(this->socket) + "]") + (enable ? "enable" : "disable"));
	if ( this->evs )
		return ( this->evs->enableEventWrite( this, this->socket, enable ) );
	return ( 0 );
}

int	Client::deleteEventProcExit( pid_t pid )
{
	if ( this->evs )
		return ( this->evs->deleteEventProcExit(this, pid));
	return ( 0 );
}

int	Client::deleteEventProcTimeout( pid_t pid )
{
	if ( this->evs )
		return ( this->evs->deleteEventProcTimeout(this, pid));
	return ( 0 );
}

