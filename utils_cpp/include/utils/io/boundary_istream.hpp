/*
 * boundary_istream.hpp
 *
 *  Created on: 29/04/2011
 *      Author: Alan
 */

#ifndef BOUNDARY_ISTREAM_HPP_
#define BOUNDARY_ISTREAM_HPP_

#include <utils/utils_defs.hpp>
#include <iostream>

namespace utils {

namespace io {

class boundary_istream : public std::istream {
public:
	virtual ~boundary_istream() { }
};

}  // namespace io


}  // namespace utils


#endif /* BOUNDARY_ISTREAM_HPP_ */
