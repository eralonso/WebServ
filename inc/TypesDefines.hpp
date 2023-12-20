/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TypesDefines.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:23:45 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/20 17:52:08 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _TYPESDEFINES_HPP_
# define _TYPESDEFINES_HPP_

# include <string>
# include <map>
# include <vector>
# include <set>

class Location;
class Directives;
class ActionMask;
class CgisMap;
class Server;

typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::set< Location * >					LocationsSet;
typedef std::vector< Server >					ServersVector;
typedef std::pair< std::string, int >			ListenPair;
typedef std::pair< int, std::string >			ReturnPair;
// typedef std::map< std::string, std::string >	CgiMap;

#endif
