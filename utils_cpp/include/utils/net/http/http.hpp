/*
 * http.hpp
 *
 *  Created on: 26/04/2011
 *      Author: Alan
 */

#ifndef HTTP_HPP_
#define HTTP_HPP_

#include <utils/utils_defs.hpp>
#include <iostream>

namespace utils {

namespace net {

namespace http {

/*
 * Constants for METHODS
 */
#define M_GET       "GET"
#define M_POST      "POST"
#define M_PUT       "PUT"
#define M_DELETE    "DELETE"
#define M_HEAD      "HEAD"
#define M_OPTIONS   "OPTIONS"
#define M_TRACE     "TRACE"
#define M_CONNECT   "CONNECT"


class http_request;
class http_response;

class http_request_parser;
class http_response_parser;

class request_ostream;

class multipart_handler {
public:
	virtual ~multipart_handler() { }
	virtual void handle(http_request* request, const std::string& type, const std::string& param_name, const std::string& filename, const std::string& content_type, std::istream& stream) = 0;
};

}  // namespace http

}  // namespace net

}  // namespace utils


#endif /* HTTP_HPP_ */
