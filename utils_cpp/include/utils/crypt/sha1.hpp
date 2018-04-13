/*
 * sha1.hpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#ifndef SHA1_HPP_
#define SHA1_HPP_

#include <utils/crypt/hasher.hpp>

namespace utils {

namespace crypt {

#define SHA1_SIZE 20

class SHA1Hasher : public Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
private:
    t_dword _hash[SHA1_SIZE/4];
    t_dword _length_low;
    t_dword _length_high;
    int _block_index;
    t_byte _block[64];
    int computed;
    int corrupted;
    void _pad();
    void _process();
public:

	SHA1Hasher();

	virtual ~SHA1Hasher();

	virtual void update(const byte_type* src, size_type slength);

	virtual void finalize(byte_type* dest);

	virtual void init();

	virtual size_type digest_size() const;

};

} // crypt

} // utils


#endif /* SHA1_HPP_ */
