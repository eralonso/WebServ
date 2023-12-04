/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:23 by omoreno-          #+#    #+#             */
/*   Updated: 2023/12/04 18:57:25 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ActionMask.hpp>

ActionMask::ActionMask( void ): allowed( \
						static_cast< t_action >( 0 ) ) {}

ActionMask::ActionMask( t_action value ): allowed( value ) {}

ActionMask::~ActionMask( void ) {}

ActionMask::ActionMask( const ActionMask& b ): allowed( b.allowed ) {}

ActionMask&	ActionMask::operator=( const ActionMask& b )
{
	allowed = b.allowed;
	return ( *this );
}

bool	ActionMask::isAction( int method ) const
{
	int	actions[ 3 ] = { ActionMask::GET, ActionMask::POST, ActionMask::DELETE };

	for ( int i = 0; i < 3; i++ )
		if ( actions[ i ] == method )
			return ( true );
	return ( false );
}

void	ActionMask::setAllowed( ActionMask::t_action allowed )
{
	this->allowed = allowed;
}

ActionMask::t_action	ActionMask::getAllowed( void ) const
{
	return ( allowed );
}

void	ActionMask::setAction( int method )
{
	if ( isAction( method ) > 0 )
		allowed = static_cast< t_action >( allowed | method );
}

void	ActionMask::unsetAction( int method )
{
	if ( isAction( method ) > 0 )
		allowed = static_cast< t_action >( allowed & ~method );
}

void	ActionMask::unsetAllActions( void )
{
	allowed = static_cast< t_action >( allowed & 0 );
}

bool	ActionMask::getAction( int method ) const
{
	if ( isAction( method ) > 0 )
		return ( allowed & method );
	return ( false );
}

//void	ActionMask::setGetAction( bool value )
//{
//	if ( value )
//		allowed = static_cast< t_action >( allowed | ActionMask::GET );
//	else
//		allowed = static_cast< t_action >( allowed & ~ActionMask::GET );
//}

//bool	ActionMask::getGetAction( void ) const
//{
//	return ( ( allowed & ActionMask::GET ) != 0 );
//}

//void	ActionMask::setPostAction( bool value )
//{
//	if ( value )
//		allowed = static_cast< t_action >( allowed | ActionMask::POST );
//	else
//		allowed = static_cast< t_action >( allowed & ~ActionMask::POST );
//}

//bool	ActionMask::getPostAction( void ) const
//{
//	return ( ( allowed & ActionMask::POST ) != 0 );
//}

//void	ActionMask::setDeleteAction( bool value )
//{
//	if ( value )
//		allowed = static_cast< t_action >( allowed | ActionMask::DELETE );
//	else
//		allowed = static_cast< t_action >( allowed & ~ActionMask::DELETE );
//}

//bool	ActionMask::getDeleteAction( void ) const
//{
//	return ( ( allowed & ActionMask::DELETE ) != 0 );
//}
