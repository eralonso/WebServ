/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:08 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/02 15:57:35 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ACTIONMASK_HPP_
# define _ACTIONMASK_HPP_

# include <string>

class ActionMask
{
public:
	typedef enum e_action
	{
		INVALID = 0,
		GET = 1,
		POST = 2,
		DELETE = 4
	}	t_action;
private:
	t_action	allowed;
	static int			actions[ 3 ];
	static std::string	stringActions[ 3 ];
public:
	ActionMask( void );
	ActionMask( t_action value );
	~ActionMask( void );
	ActionMask( const ActionMask& b );
	ActionMask&	operator=( const ActionMask& b );
	static bool		isAction( int method );
	static int		whichAction( std::string method );
	void		setAllowed( t_action allowed );
	t_action	getAllowed( void ) const;
	void		setAction( int method );
	void		unsetAction( int method );
	void		unsetAllActions( void );
	bool		getAction( int method ) const;
	//void		setGetAction( bool value );
	//bool		getGetAction( void );
	//void		setPostAction( bool value );
	//bool		getPostAction( void );
	//void		setDeleteAction( bool value );
	//bool		getDeleteAction( void );
};


#endif
