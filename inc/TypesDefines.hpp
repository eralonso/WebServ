/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TypesDefines.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:23:45 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/14 12:15:16 by omoreno-         ###   ########.fr       */
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

typedef std::map< unsigned int, std::string >										ErrorPagesMap;
typedef std::set< Location *, bool ( * )( const Location *, const Location * ) >	LocationsSet;
typedef std::vector< Server >														ServersVector;
typedef std::pair< std::string, int >												ListenPair;
typedef std::pair< int, std::string >												ReturnPair;

#endif
