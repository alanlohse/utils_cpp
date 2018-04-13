/*
 * exception.hpp
 *
 *  Created on: 31/03/2010
 *      Author: Alan
 */

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <utils/utils_defs.hpp>
#include <exception>
#include <string>

namespace utils {

class utils_exception : public std::exception {
private:
	std::string cause;
public:
	utils_exception() throw() {
		cause = std::exception::what();
	}
	utils_exception(const std::string& _cause) throw() :
		cause(_cause)
	{
	}
	virtual ~utils_exception() throw() { }
	virtual const char* what() const throw() {
		return cause.c_str();
	}
};

class boundary_exceeded_exception : public utils_exception {
public:
	boundary_exceeded_exception() throw() :
		utils_exception()
	{
	}
	boundary_exceeded_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

class invalid_argument_exception : public utils_exception {
public:
	invalid_argument_exception() throw() :
		utils_exception()
	{
	}
	invalid_argument_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

std::string get_error_message(int err);

}

#endif /* EXCEPTION_HPP_ */
