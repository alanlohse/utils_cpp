/*
 * hasher.hpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#ifndef HASHER_HPP_
#define HASHER_HPP_

#include <utils/utils_defs.hpp>

namespace utils {

namespace crypt {

class Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
public:

	virtual ~Hasher() { }

	virtual void update(const byte_type* src, size_type slength) = 0;

	virtual void finalize(byte_type* dest) = 0;

	virtual void init() = 0;

	virtual size_type digest_size() const { return 0; }

};

} // crypt

} // utils

#endif /* HASHER_HPP_ */
