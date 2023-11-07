/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:08 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 12:40:17 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ACTIONMASK_HPP_
# define  _ACTIONMASK_HPP_

class ActionMask
{
public:
	typedef enum s_action
	{
		GET = 1,
		POST = 2,
		DELETE = 4
	}	t_action;
private:
	t_action	allowed;
public:
	ActionMask();
	ActionMask(t_action value);
	~ActionMask();
	void		setAllowed(t_action allowed);
	t_action	getAllowed(void);
	void		setGetAction(bool value);
	bool		getGetAction(void);
	void		setPostAction(bool value);
	bool		getPostAction(void);
	void		setDeleteAction(bool value);
	bool		getDeleteAction(void);
};


#endif