/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ActionMask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:06:23 by omoreno-          #+#    #+#             */
/*   Updated: 2024/01/12 15:22:47 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ActionMask.hpp>

int			ActionMask::actions[ METHODS_NB ] = {
					ActionMask::GET,
					ActionMask::POST,
					ActionMask::PUT,
					ActionMask::DELETE,
					ActionMask::HEAD };
std::string	ActionMask::stringActions[ METHODS_NB ] = {
					"GET",
					"POST",
					"PUT",
					"DELETE",
					"HEAD"};

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
	for ( int i = 0; i < METHODS_NB; i++ )
		if ( ActionMask::actions[ i ] == method )
			return ( true );
	return ( false );
}

int	ActionMask::whichAction( std::string method )
{
	for ( int i = 0; i < METHODS_NB; i++ )
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
