/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestCnfg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:55:16 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/08 13:56:36 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>
#include <ServerFinder.hpp>

bool	Request::updateServerConfig( void )
{
	this->svr = ServerFinder::find( *this );
	if ( this->svr != NULL )
		return ( true );
	return ( false );
}

void	Request::updateLocation( void )
{
	//std::string	routeWithoutFile = getRouteChaineString();
	std::string	routeWithoutFile = getRoute();

	// Log::Error( "location string: " + routeWithoutFile );
	if ( this->svr != NULL )
		this->lc = this->svr->getLocationAtPath( routeWithoutFile );
}

void	Request::updateFilePaths( void )
{
	std::string	routeWithoutFile = getRoute();

	if ( this->svr == NULL )
	{
		this->filePathWrite = ConfigUtils::pathJoin( ".", this->route );
		this->filePathRead = ConfigUtils::pathJoin( ".", this->route );
		return ;
	}
	// if ( this->method == "POST" || this->method == "PUT" )
	// {
	// 	return ;
	// }
	this->filePathWrite = svr->getFinalUploadPath( routeWithoutFile, this->lc );
	this->filePathRead = svr->getFinalPath( routeWithoutFile, this->lc );
	// Log::Success( "filePathRead == " + this->filePathRead );
	// Log::Success( "filePathWrite == " + this->filePathWrite );
}

bool	Request::tryIndexFiles( std::string& file )
{
	bool		found;
	size_t		pos;

	found = false;
	if ( this->svr != NULL )
		found = this->svr->tryIndexFiles( file, this->filePathRead, this->lc );
	if ( found )
	{
		getExtensionForPath( file, this->docExt );
		pos = file.find_last_of( "/" );
		if ( pos != std::string::npos )
			this->document = file.substr( pos );
		else
			this->document = file;
	}
	return ( found );
}

bool	Request::findReturnUri( int& uriCode, std::string& uriRedirection ) const
{
	if ( this->svr != NULL )
		return ( this->svr->findReturnUri( uriCode, uriRedirection, this->lc ) );
	return ( false );
}

