/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/07 14:28:59 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ActionMask.hpp>

ActionMask::ActionMask()
{
	allowed = static_cast<t_action>(GET | POST | DELETE);
}

ActionMask::ActionMask(t_action value)
{
	allowed = value;
}

ActionMask::~ActionMask()
{
}

ActionMask::ActionMask(const ActionMask& b)
{
	allowed = b.allowed;
}

ActionMask& ActionMask::operator=(const ActionMask& b)
{
	allowed = b.allowed;
	return (*this);
}

void					ActionMask::setAllowed(ActionMask::t_action allowed)
{
	this->allowed = allowed;
}

ActionMask::t_action	ActionMask::getAllowed(void)
{
	return allowed;	
}

void		ActionMask::setGetAction(bool value)
{
	if (value)
		allowed = static_cast<t_action>(allowed | ActionMask::GET);
	else
		allowed = static_cast<t_action>(allowed & ~ActionMask::GET);
}

bool		ActionMask::getGetAction(void)
{
	return ((allowed & ActionMask::GET) != 0);
}

void		ActionMask::setPostAction(bool value)
{
	if (value)
		allowed = static_cast<t_action>(allowed | ActionMask::POST);
	else
		allowed = static_cast<t_action>(allowed & ~ActionMask::POST);
}

bool		ActionMask::getPostAction(void)
{
	return ((allowed & ActionMask::POST) != 0);
}

void		ActionMask::setDeleteAction(bool value)
{
	if (value)
		allowed = static_cast<t_action>(allowed | ActionMask::DELETE);
	else
		allowed = static_cast<t_action>(allowed & ~ActionMask::DELETE);
}

bool		ActionMask::getDeleteAction(void)
{
	return ((allowed & ActionMask::DELETE) != 0);
}

