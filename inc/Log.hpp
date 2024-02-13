/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:56:29 by omoreno-          #+#    #+#             */
/*   Updated: 2024/02/13 11:38:51 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOG_HPP_
# define _LOG_HPP_

# include <iostream>
# include <string>

# include <Defines.hpp>
# include <Utils.hpp>

class Log
{
private:
	Log(void);
	Log(const Log&);
	Log& operator=(const Log&);
	~Log(void);
private:
	static bool	resetLine;
public:
	static void	Info( const std::string str );
	static void	Warn( const std::string str );
	static void	Error( const std::string str );
	static void	Success( const std::string str );
	static void	Debug( const std::string str );
	static void	DropOneLine( void );
	static void	Timeout( std::string add );
};

#endif