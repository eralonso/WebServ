/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:10:16 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/25 11:16:15 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DEFINES_HPP_
# define _DEFINES_HPP_

# include <vector>
# include <map>
# include <string>

# define BUFFER_SIZE 3000
# define SEND_BUFFER_SIZE 6000
# define MAX_CLIENTS 3000

# define SERVER "OREGINX/1.0"

# define SPOLLFD 0
# define CPOLLFD 1

# define DEF "\033[0m"
# define RED "\033[1;91m"
# define GREEN "\033[1;92m"
# define YELLOW "\033[1;93m"
# define BLUE "\033[1;94m"
# define PURPLE "\033[1;95m"
# define CYAN "\033[1;96m"

# define ISSPACE "\t\n\v\f\r "

# define NOT_A_SEPARATOR 0
# define COMMENT_SEPARATOR 1
# define SEMICOLON_SEPARATOR 2
# define BRACET_SEPARATOR 3

typedef int										socket_t;
typedef std::vector< std::string >				StringVector;
typedef std::vector< const std::string >		ConstStringVector;
typedef std::map< const std::string, bool >		ConstStringBoolMap;
typedef std::map< std::string, bool >			StringBoolMap;
typedef std::pair< const std::string, bool >	ConstStringBoolPair;
typedef std::pair< std::string, bool >			StringBoolPair;

#endif
