/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:23 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/02 15:57:43 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ActionMask.hpp>

int			ActionMask::actions[ 3 ] = { ActionMask::GET, ActionMask::POST, ActionMask::DELETE };
std::string	ActionMask::stringActions[ 3 ] = { "GET", "POST", "DELETE" };

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

bool	ActionMask::isAction( int method )
{
	for ( int i = 0; i < 3; i++ )
		if ( ActionMask::actions[ i ] == method )
			return ( true );
	return ( false );
}

int	ActionMask::whichAction( std::string method )
{
	for ( int i = 0; i < 3; i++ )
		if ( ActionMask::stringActions[ i ] == method )
			return ( actions[ i ] );
	return ( ActionMask::INVALID );
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
