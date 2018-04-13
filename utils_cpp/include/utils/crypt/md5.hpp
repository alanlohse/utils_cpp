/*
 * md5.hpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#ifndef MD5_HPP_
#define MD5_HPP_

#include <utils/crypt/hasher.hpp>

namespace utils {

namespace crypt {

class MD5Hasher : public Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
private:
	t_dword state[4];
	t_dword count[2];
	byte_type buffer[64];
	void transform(const byte_type* src);
public:

	MD5Hasher();

	virtual ~MD5Hasher();

	virtual void update(const byte_type* src, size_type slength);

	virtual void finalize(byte_type* dest);

	virtual void init();

	virtual size_type digest_size() const;

};

} // crypt

} // utils

#endif /* MD5_HPP_ */
