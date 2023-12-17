/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/08 19:39:42 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>
#include <Receptionist.hpp>
#include <ConfigParser.hpp>

int	main( int argc, char **argv )
{
	WSSignals::signalHandler();
	try
	{
		ConfigParser	configParser( argc, argv );
		Receptionist	receptionist( configParser.getServers() );
		
		return ( receptionist.mainLoop() );
	}
	catch ( const std::exception &e )
	{
		Log::Error( e.what() );
	}
	return ( 0 );
}
