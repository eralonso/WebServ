/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigApply.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 16:24:56 by eralonso          #+#    #+#             */
/*   Updated: 2024/02/14 11:46:22 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CONFIGAPPLY_HPP_
# define _CONFIGAPPLY_HPP_

# include <Defines.hpp>
# include <ConfigUtils.hpp>
# include <Utils.hpp>

namespace ConfigApply
{
	const std::string	applyAlias( const std::string& path, \
									const std::string& route, \
	   								const std::string alias );
	const std::string	applyRoot( const std::string& path, \
									const std::string root );
}

#endif
