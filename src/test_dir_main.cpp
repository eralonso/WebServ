/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dir_main.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:15:43 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/13 13:44:32 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <Utils.hpp>
#include <FolderLs.hpp>

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		Log::Error("Usage " + std::string(argv[0]) + "<path> <route>");
		return (1);
	}
	std::string	res;
	if (FolderLs::getLs(res, std::string(argv[1]), std::string(argv[2])))
	{
		std::cout << res;
		return (0);
	}
	Log::Error(std::string("Cant open directory ") + std::string(argv[1]));
	return (1);
}
