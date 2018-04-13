/*
 * time.hpp
 *
 *  Created on: 25/03/2010
 *      Author: alan.lohse
 */

#ifndef TIME_HPP_
#define TIME_HPP_

#include <utils/utils_defs.hpp>

namespace utils {

class Time {
public:
	static t_bigint nanoseconds();

	static t_bigint zeroTime();

	static t_bigint milliseconds();

};

}

#endif /* TIME_HPP_ */
