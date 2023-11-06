/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/06 12:39:28 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>
#include <Receptionist.hpp>

int	main( void )
{
	WSSignals::signalHandler();
	Receptionist receptionist;

	return (receptionist.mainLoop());
}
