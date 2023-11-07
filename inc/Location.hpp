/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:51 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 15:15:39 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LOCATION_HPP_
# define _LOCATION_HPP_
# include <vector>
# include <string>
# include <RootDir.hpp>
# include <ActionMask.hpp>
# include <CGIService.hpp>

class Location
{
private:
	std::string				path;
	RootDir					rootDir;
	ActionMask				actionMask;
	std::vector<CGIService>	servicesCGI;
public:
	Location();
	Location(std::string head, std::string body);
	~Location();
	Location(const Location& b);
	Location& 					operator=(const Location& b);
	std::string					getPath(void);
	RootDir						getRootDir(void);
	ActionMask					getActionMask(void);
	std::vector<CGIService>		getServicesCGI(void);
};

#endif