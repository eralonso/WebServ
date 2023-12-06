/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:08 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 16:56:34 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ACTIONMASK_HPP_
# define _ACTIONMASK_HPP_

class ActionMask
{
public:
	typedef enum e_action
	{
		GET = 1,
		POST = 2,
		DELETE = 4
	}	t_action;
private:
	t_action	allowed;
public:
	ActionMask( void );
	ActionMask( t_action value );
	~ActionMask( void );
	ActionMask( const ActionMask& b );
	ActionMask&	operator=( const ActionMask& b );
	bool		isAction( int method ) const;
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
