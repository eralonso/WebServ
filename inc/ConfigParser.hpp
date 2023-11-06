/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:49:11 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/06 12:52:16 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

class ConfigParser
{
private:
	/* data */
public:
	ConfigParser(int argc, char **argv);
	~ConfigParser();
};

#endif