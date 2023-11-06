/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WSSignals.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:59:50 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 12:39:16 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WSSIGNALS_HPP_
# define _WSSIGNALS_HPP_

# include <Utils.hpp>

class WSSignals
{
	public:
		static bool	isSig;

		static void	sighandler( int );
		static void	signalHandler( void );
};

#endif