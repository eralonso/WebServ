/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesParserInit.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:45:43 by eralonso          #+#    #+#             */
/*   Updated: 2023/12/01 19:41:46 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <DirectivesParser.hpp>

const std::string	DirectivesParser::_directivesListAux[ SIZE_DIRECTIVES + 1 ] = 
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

const std::string	DirectivesParser::_simpleDirectivesListAux[ \
		  									SIZE_SIMPLE_DIRECTIVES + 1 ] = 
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

const std::string	DirectivesParser::_complexDirectivesListAux[ \
		  									SIZE_COMPLEX_DIRECTIVES + 1 ] = 
{
	"server",
	"location"
};

const ConstStringVector	DirectivesParser::_directivesList( _directivesListAux, \
											_directivesListAux \
											+ SIZE_DIRECTIVES );

const ConstStringVector	DirectivesParser::_simpleDirectivesList( \
											_simpleDirectivesListAux, \
	   										_simpleDirectivesListAux \
											+ SIZE_SIMPLE_DIRECTIVES );

const ConstStringVector	DirectivesParser::_complexDirectivesList( \
											_complexDirectivesListAux, \
											_complexDirectivesListAux \
											+ SIZE_COMPLEX_DIRECTIVES );

const ConstStringBoolPair	DirectivesParser::_canRepeatDirectiveListAux[ \
		  									SIZE_DIRECTIVES + 1 ] =
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
	std::make_pair<	const std::string, bool > ( "server", true ),
	std::make_pair<	const std::string, bool > ( "location", true )
};

const ConstStringBoolMap	DirectivesParser::_canRepeatDirectiveList( \
											_canRepeatDirectiveListAux, \
											_canRepeatDirectiveListAux \
											+ SIZE_DIRECTIVES );

DirectivesParser::t_parseSimpleDirectiveArray	DirectivesParser::_parseSimple = \
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

DirectivesParser::t_parseComplexDirectiveArray	DirectivesParser::_parseComplex = \
{
	&DirectivesParser::parseServer,
	&DirectivesParser::parseLocation
};
