/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:41:41 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 16:07:55 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <DirectivesParser.hpp>

const std::string	DirectivesParser::_DirectivesArray[ SIZE_DIRECTIVES + 1 ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size",
	"index",
	"autoindex",
	"alias",
	"rewrite",
	"allow_methods",
	"cgi",
	"server",
	"location"
};

const std::string	DirectivesParser::_SimpleDirectivesArray[ SIZE_SIMPLE_DIRECTIVES + 1 ] = 
{
	"root",
	"listen",
	"server_name",
	"error_page",
	"client_max_body_size"
	"index",
	"autoindex",
	"alias",
	"rewrite",
	"allow_methods",
	"cgi"
};

const std::string	DirectivesParser::_ComplexDirectivesArray[ SIZE_COMPLEX_DIRECTIVES + 1 ] = 
{
	"server",
	"location"
};

const std::vector< const std::string >	DirectivesParser::DirectivesVector( \
						_DirectivesArray, _DirectivesArray + SIZE_DIRECTIVES );

const std::vector< const std::string >	DirectivesParser::SimpleDirectivesVector( \
											_SimpleDirectivesArray, \
	   										_SimpleDirectivesArray + SIZE_SIMPLE_DIRECTIVES );

const std::vector< const std::string >	DirectivesParser::ComplexDirectivesVector( \
											_ComplexDirectivesArray, \
											_ComplexDirectivesArray + SIZE_COMPLEX_DIRECTIVES );

const std::pair< const std::string, bool >	DirectivesParser::_canRepeatDirectivePair[ SIZE_DIRECTIVES + 1 ] =
{
	std::make_pair<	const std::string, bool > ( "root", false ),
	std::make_pair<	const std::string, bool > ( "listen", false ),
	std::make_pair<	const std::string, bool > ( "server_name", true ),
	std::make_pair<	const std::string, bool > ( "error_page", true ),
	std::make_pair<	const std::string, bool > ( "client_max_body_size", false ),
	std::make_pair<	const std::string, bool > ( "index", true ),
	std::make_pair<	const std::string, bool > ( "autoindex", false ),
	std::make_pair<	const std::string, bool > ( "alias", false ),
	std::make_pair<	const std::string, bool > ( "rewrite", false ),
	std::make_pair<	const std::string, bool > ( "allow_methods", false ),
	std::make_pair<	const std::string, bool > ( "cgi", true ),
	std::make_pair<	const std::string, bool > ( "server", false )
	std::make_pair<	const std::string, bool > ( "location", true )
};

const std::map< const std::string, bool >	DirectivesParser::_canRepeatDirective( \
										_canRepeatDirectivePair, \
										_canRepeatDirectivePair + SIZE_DIRECTIVES );

DirectivesParser::t_parseSimpleDirectiveArray	DirectivesParser::_ParseSimple = \
{
	&DirectivesParser::parseRoot,
	&DirectivesParser::parseListen,
	&DirectivesParser::parseServerNames,
	&DirectivesParser::parseErrorPage,
	&DirectivesParser::parseClientMaxBodySize,
	&DirectivesParser::parseIndex,
	&DirectivesParser::parseAutoindex,
	&DirectivesParser::parseAlias,
	&DirectivesParser::parseRewrite,
	&DirectivesParser::parseAllowMethods,
	&DirectivesParser::parseCgi
};

DirectivesParser::t_parseComplexDirectiveArray	DirectivesParser::_ParseComplex = \
{
	&DirectivesParser::parseServer,
	&DirectivesParser::parseLocation
};


DirectivesParser::DirectivesParser( void ) {}

DirectivesParser::DirectivesParser( const DirectivesParser& dp )
{
	( void )dp;
}

DirectivesParser::~DirectivesParser( void ) {}

DirectivesParser&	DirectivesParser::operator=( const DirectivesParser& dp )
{
	( void )dp;
	return ( *this );
}

Directives	DirectivesParser::parseDirectives( std::string content, \
					StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives )
{
	std::string							head;
	std::string							body;
	int									type;
	std::string							name;
	Directives							d;
	std::map< const std::string, bool >	isSet;

	for ( int i = 0; i < SIZE_DIRECTIVES; i++ )
		isSet[ DirectivesVector[ i ] ] = false;
	while ( content.length() > 0 )
	{
		if ( type = TreeSplit::get_pair( head, body, content ) )
		{
			name = head.substr( 0, head.find( ISSPACE ) );
			checkValidDirective( name );
			checkValidSeparator( type, name, separators[ type - 1 ] );
			checkDuplicateDirective( name, isSet );
			isSet[ name ] = true;
			parseDirective( head, body, type, d, allowedSimpleDirectives, \
					allowedComplexDirectives );
		}
		else if ( content.length() > 0 )
			throw std::logic_error( "Unxpected \"}\" or end of file" );
		head.clear();
		body.clear();
	}
	return ( d );
}

void	DirectivesParser::parseDirective( std::string head, std::string body, \
					Directives& d, StringVector allowedSimpleDirectives, \
					StringVector allowedComplexDirectives )
{
	int	idx;

	if ( ( idx = isSimpleDirective( head, allowedSimpleDirectives ) ) >= 0 )
		_ParseSimple[ idx ]( body, d );
	else if ( ( idx = isComplexDirective( head, allowedComplexDirectives ) ) >= 0 )
		_ParseComplex[ idx ]( head, body, d );
}

void	DirectivesParser::checkValidDirective( std::string directive )
{
	if ( std::find( DirectivesVector.begin(), DirectivesVector.end(), directive ) \
			== DirectivesVector.end() )
		throw std::logic_error( UNKNOWN_DIRECTIVE( directive ) );
}

void	DirectivesParser::checkValidSeparator( int type, \
						std::string directive, std::string separator )
{
	static int	separators[ 2 ] = { ";", "{" };

	if ( type == separators[ type - 1 ] )
		throw std::logic_error( "\"" + directive \
				+ "\" invalid separator for this directive, expected \"" \
				+ separators[ type - 1 ] + "\"" );
}

void	DirectivesParser::checkDuplicateDirective( std::string directive, \
	 					std::map< const std::string, bool > isSet )
{
	if ( isSet[ directive ] == true && _canRepeatDirective[ directive ] == false )
		throw std::logic_error( "\"" + directive + "\" directive is duplicate" );
}
