/*
 * http_request.hpp
 *
 *  Created on: 26/04/2011
 *      Author: Alan
 */

#ifndef HTTP_REQUEST_HPP_
#define HTTP_REQUEST_HPP_

#include <utils/net/http/http.hpp>
#include <utils/net/url.hpp>
#include <utils/net/socket.hpp>
#include <string>
#include <iostream>
#include <utils/containers/hash_map.hpp>

namespace utils {

namespace net {

namespace http {

using containers::hashmap;

struct http_request_data {
	typedef std::vector<std::string, DEFAULT_ALLOCATOR<std::string> > string_list;
	typedef hashmap<std::string, string_list, hash<const std::string&>, equal_to_nocase<const std::string&> > parameter_map;
	typedef hashmap<std::string, std::string, hash<const std::string&>, equal_to_nocase<const std::string&> > headers_map;

	parameter_map _M_parameters;
	headers_map _M_headers;
	multipart_handler* _M_multipart_handler;
	socket _M_socket;
	int _M_refs;

	http_request_data* add_ref() {
		++_M_refs;
		return this;
	}
	int rem_ref() {
		return --_M_refs;
	}
	bool is_leak() const {
		return _M_refs <= 0;
	}
};

class http_request {
protected:

public:

};

}

}

}

#endif /* HTTP_REQUEST_HPP_ */
