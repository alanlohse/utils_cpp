/*
 * math_exception.hpp
 *
 *  Created on: 31/03/2010
 *      Author: Alan
 */

#ifndef MATH_EXCEPTION_HPP_
#define MATH_EXCEPTION_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>

namespace utils {

namespace math {

class math_exception : public utils_exception {
public:
	math_exception() throw() :
		utils_exception()
	{
	}
	math_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

}

}

#endif /* MATH_EXCEPTION_HPP_ */
