/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigApply.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 16:24:56 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/24 17:44:26 by eralonso         ###   ########.fr       */
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
	const std::string	applyAlias( const StringVector& path, \
									const StringVector& route, \
	   								const std::string alias );
	const std::string	applyRoot( const std::string& path, \
									const std::string root );
	const std::string	applyUploadStore( const std::string& path, \
											const std::string uploadRoot );
}

#endif
