/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TypesDefines.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:23:45 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/08 19:39:37 by eralonso         ###   ########.fr       */
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
class CGIService;
class Server;

typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::set< Location * >					LocationsSet;
typedef std::vector< Server >					ServersVector;
typedef std::pair< std::string, int >			ListenPair;
typedef std::pair< int, std::string >			ReturnPair;
typedef std::map< std::string, CGIService >		CgiMap;

#endif
