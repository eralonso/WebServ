/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:44:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/06 12:53:28 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WSSignals.hpp>
#include <Receptionist.hpp>
#include <ConfigParser.hpp>


int	main(int argc, char **argv)
{
	WSSignals::signalHandler();
	ConfigParser configParser(argc, argv);
	Receptionist receptionist;

	return (receptionist.mainLoop());
}
