/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TypesDefines.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 12:23:45 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/01 13:16:08 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _TYPESDEFINES_HPP_
# define _TYPESDEFINES_HPP_

# include <string>
# include <map>
# include <vector>

class Location;
class Directives;
class ActionMask;
class CGIService;
class Server;

typedef std::map< unsigned int, std::string >	ErrorPagesMap;
typedef std::vector< Location >					LocationsVector;
typedef std::vector< Server >					ServersVector;
typedef std::pair< std::string, int >			ListenPair;
typedef std::pair< int, std::string >			ReturnPair;
typedef std::vector< CGIService >				CgiVector;

#endif
