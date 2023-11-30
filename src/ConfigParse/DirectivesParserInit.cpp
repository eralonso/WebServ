/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParserInit.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:45:43 by eralonso          #+#    #+#             */
/*   Updated: 2023/11/30 18:59:23 by eralonso         ###   ########.fr       */
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
	"return",
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
	"return",
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
	std::make_pair<	const std::string, bool > ( "return", false ),
	std::make_pair<	const std::string, bool > ( "allow_methods", false ),
	std::make_pair<	const std::string, bool > ( "cgi", true ),
	std::make_pair<	const std::string, bool > ( "server", true )
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
	&DirectivesParser::parseReturn,
	&DirectivesParser::parseAllowMethods,
	&DirectivesParser::parseCgi
};

DirectivesParser::t_parseComplexDirectiveArray	DirectivesParser::_ParseComplex = \
{
	&DirectivesParser::parseServer,
	&DirectivesParser::parseLocation
};
