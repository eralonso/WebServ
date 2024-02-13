/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:00:10 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 11:40:06 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

bool	Log::resetLine = false;

Log::Log(void) {}

Log::~Log(void) {}

Log::Log(const Log&) {}

Log&	Log::operator=(const Log&) { return (*this); }

void	Log::Info( const std::string str )
{
	Log::resetLine = false;
	std::cout << BLUE << "Info: " << str << DEF << std::endl;
}

void	Log::Warn( const std::string str )
{
	Log::resetLine = false;
	std::cout << YELLOW << "Info: " << str << DEF << std::endl;
}

void	Log::Error( const std::string str )
{
	Log::resetLine = false;
	std::cerr << RED << "Error: " << str << DEF << std::endl;
}

void	Log::Success( const std::string str )
{
	Log::resetLine = false;
	std::cout << GREEN << "Success: "<< str << DEF << std::endl;
}	

void	Log::Debug( const std::string str )
{
	#ifdef LOG_DEBUG
	std::cout << PURPLE << "Debug: "<< str << DEF << std::endl;
	Log::resetLine = true;
	#else
	(void)str;
	#endif
}

void	Log::Timeout( std::string add )
{
	static std::string	starWait = "|/-\\";
	static size_t		starLength = starWait.length();
	static size_t		count;

	if ( count == starLength )
		count = 0;
	if ( Log::resetLine )
		Log::DropOneLine();
	std::cout << CYAN"Waiting events " << starWait[ count ] << add << DEF << std::endl;
	count++;
	Log::resetLine = true;
}

void	Log::DropOneLine( void )
{
	std::cout << GO_UP_LINES( 1 ) << CLEAR_LINE;
}
