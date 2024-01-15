/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinesFunctions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:16:59 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/15 09:15:55 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DEFINES_FUNCTIONS_HPP_
# define _DEFINES_FUNCTIONS_HPP_

# include <limits>
# include <Utils.hpp>

# define MAX_NUMERIC_LIMITS( t ) std::numeric_limits< t >::max()

# define MAX_NUMERIC_LIMITS_STR( t ) SUtils::longToString( \
		MAX_NUMERIC_LIMITS( t ) )

# define COMPARE_NBR_MAX_STR( s, t ) SUtils::compareNumbersAsStrings( s, \
		MAX_NUMERIC_LIMITS_STR( t ) )

#endif
