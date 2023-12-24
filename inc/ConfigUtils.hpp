/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 16:52:20 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 17:49:54 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGUTILS_HPP_
# define _CONFIGUTILS_HPP_

# include <string>

# include <Defines.hpp>
# include <Utils.hpp>

namespace ConfigUtils
{
	int			comparePathReference( const std::string path, \
								const std::string reference );
	int			comparePathReference( const StringVector path, \
								const StringVector reference );
	std::string	pathJoin( const std::string str1, const std::string str2 );
}

#endif
