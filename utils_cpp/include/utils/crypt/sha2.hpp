/*
 * sha256.hpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#ifndef SHA256_HPP_
#define SHA256_HPP_

#include <utils/crypt/hasher.hpp>

namespace utils {

namespace crypt {

class SHA256Hasher: public Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
private:
	byte_type data[64];
	t_dword datalen;
	t_dword bitlen[2];
	t_dword state[8];
	void transform(byte_type* data);
public:

	SHA256Hasher();

	virtual ~SHA256Hasher();

	virtual void update(const byte_type* src, size_type slength);

	virtual void finalize(byte_type* dest);

	virtual void init();

	virtual size_type digest_size() const;

};

class SHA384Hasher: public Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
protected:
	t_qword _h[8];
	t_byte	_buffer[128];
	t_qword _total[2];
	virtual void _copy_output(byte_type* dest);
	void process(const byte_type* data);
public:

	SHA384Hasher();

	virtual ~SHA384Hasher();

	virtual void update(const byte_type* src, size_type slength);

	virtual void finalize(byte_type* dest);

	virtual void init();

	virtual size_type digest_size() const;

};


class SHA512Hasher: public SHA384Hasher {
public:
	typedef unsigned char byte_type;
	typedef size_t size_type;
protected:
	virtual void _copy_output(byte_type* dest);
public:

	SHA512Hasher();

	virtual ~SHA512Hasher();

	virtual void init();

	virtual size_type digest_size() const;

};


} // crypt

} // utils

#endif /* SHA256_HPP_ */
