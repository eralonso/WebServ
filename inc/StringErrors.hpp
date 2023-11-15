/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringErrors.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 17:14:06 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/15 16:54:45 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _STRINGERRORS_HPP_
# define _STRINGERRORS_HPP_

# define UNKNOWN_DIRECTIVE( str ) ( "Unkown directive \"" + str + "\"" )
# define INVALID_NUMBER_ARGUMENTS_DIRECTIVE( str ) ( "Invalid number of arguments in \"" + str + "\" directive" )
# define INVALID_VALUE_DIRECTIVE( str ) ( "\"" + str + "\" directive invalid value" )

#endif
