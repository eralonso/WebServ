/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectivesDefines.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 16:57:21 by eralonso          #+#    #+#             */
/*   Updated: 2024/01/10 18:46:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DIRECTIVESDEFINES_HPP_
# define _DIRECTIVESDEFINES_HPP_

# define SIZE_SIMPLE_DIRECTIVES 14
# define SIZE_COMPLEX_DIRECTIVES 2
# define SIZE_DIRECTIVES ( SIZE_SIMPLE_DIRECTIVES + SIZE_COMPLEX_DIRECTIVES )

# define PARSE_LISTEN_ERRORS_SIZE 3
# define IP_VALID_CHARS "0123456789."
# define MIN_ERROR_CODE 300 
# define MAX_ERROR_CODE 599 

# define HTTP_PREFIX "http://"
# define HTTPS_PREFIX "https://"

//Success Codes
# define HTTP_OK_CODE 200
# define HTTP_CREATED_CODE 201
# define HTTP_ACCEPTED_CODE 202
# define HTTP_NO_CONTENT_CODE 204

//Redirect Codes
# define HTTP_MOVED_TEMPORARILY_CODE 302
# define HTTP_SEE_OTHER_CODE 303

//Error Codes
# define HTTP_BAD_REQUEST_CODE 400
# define HTTP_FORBIDDEN_CODE 403
# define HTTP_NOT_FOUND_CODE 404
# define HTTP_NOT_ALLOWED_CODE 405
# define HTTP_INTERNAL_SERVER_ERROR_CODE 500
# define HTTP_GATEWAY_TIMEOUT_CODE 504
# define HTTP_VERSION_NOT_SUPPORTED_CODE 505

#endif
