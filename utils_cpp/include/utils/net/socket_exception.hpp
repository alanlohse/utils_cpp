/*
 * socket_exception.hpp
 *
 *  Created on: 24/04/2011
 *      Author: Alan
 */

#ifndef SOCKET_EXCEPTION_HPP_
#define SOCKET_EXCEPTION_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>

namespace utils {

namespace net {

class socket_exception : public utils_exception {
public:
	socket_exception() throw() :
		utils_exception()
	{
	}
	socket_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
	socket_exception(int error) throw() :
		utils_exception(get_error_message(error))
	{
	}
};

}

}

#endif /* SOCKET_EXCEPTION_HPP_ */
