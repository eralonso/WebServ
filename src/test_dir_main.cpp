/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dir_main.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:15:43 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/15 13:17:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <Utils.hpp>
#include <FolderLs.hpp>
#include <errno.h>

int main(int argc, char const *argv[])
{
	FolderLs::t_error err;

	if (argc != 3)
	{
		Log::Error("Usage " + std::string(argv[0]) + " <path> <route>");
		return (1);
	}
	std::string	res;
	err = FolderLs::getLs(res, std::string(argv[1]), std::string(argv[2]));
	if (err == LsEntry::NONE)
	{
		std::cout << "Result:\n";
		std::cout << res;
		return (0);
	}
	else if (err == LsEntry::CANTOPENDIR)
		Log::Error(std::string("Cant open directory ") + std::string(argv[1]));
	else
		Log::Error(std::string("Unknown error when opening ") + std::string(argv[1]));
	return (err);
}
