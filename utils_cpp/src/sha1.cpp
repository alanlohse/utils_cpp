/*
 * sha1.cpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#include <utils/crypt/sha1.hpp>

using namespace utils::crypt;

SHA1Hasher::SHA1Hasher() {
	init();
}

SHA1Hasher::~SHA1Hasher() {
}

#define SHA1CircularShift(bits,word) \
                (((word) << (bits)) | ((word) >> (32-(bits))))

void SHA1Hasher::_process() {
	const t_dword K[] = { /* Constants defined in SHA-1   */
	0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };
	int t; /* Loop counter                */
	t_dword temp; /* Temporary word value        */
	t_dword W[80]; /* Word sequence               */
	t_dword A, B, C, D, E; /* Word buffers                */

	/*
	 *  Initialize the first 16 words in the array W
	 */
	for (t = 0; t < 16; t++) {
		W[t] = _block[t * 4] << 24;
		W[t] |= _block[t * 4 + 1] << 16;
		W[t] |= _block[t * 4 + 2] << 8;
		W[t] |= _block[t * 4 + 3];
	}

	for (t = 16; t < 80; t++) {
		W[t] = SHA1CircularShift(1,
				W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
	}

	A = _hash[0];
	B = _hash[1];
	C = _hash[2];
	D = _hash[3];
	E = _hash[4];

	for (t = 0; t < 20; t++) {
		temp = SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t]
				+ K[0];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 20; t < 40; t++) {
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++) {
		temp = SHA1CircularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E
				+ W[t] + K[2];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++) {
		temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	_hash[0] += A;
	_hash[1] += B;
	_hash[2] += C;
	_hash[3] += D;
	_hash[4] += E;

	_block_index = 0;
}

void SHA1Hasher::_pad() {
	/*
	 *  Check to see if the current message block is too small to hold
	 *  the initial padding bits and length.  If so, we will pad the
	 *  block, process it, and then continue padding into a second
	 *  block.
	 */
	if (_block_index > 55) {
		_block[_block_index++] = 0x80;
		while (_block_index < 64) {
			_block[_block_index++] = 0;
		}

		_process();

		while (_block_index < 56) {
			_block[_block_index++] = 0;
		}
	} else {
		_block[_block_index++] = 0x80;
		while (_block_index < 56) {
			_block[_block_index++] = 0;
		}
	}

	/*
	 *  Store the message length as the last 8 octets
	 */
	_block[56] = _length_high >> 24;
	_block[57] = _length_high >> 16;
	_block[58] = _length_high >> 8;
	_block[59] = _length_high;
	_block[60] = _length_low >> 24;
	_block[61] = _length_low >> 16;
	_block[62] = _length_low >> 8;
	_block[63] = _length_low;

	_process();
}

void SHA1Hasher::update(const byte_type* src, size_type slength) {
	if (computed) {
		corrupted = true;
		return;
	}
	if (corrupted)
		return;
	while (slength-- && !corrupted) {
		_block[_block_index++] = (*src & 0xFF);

		_length_low += 8;
		if (_length_low == 0) {
			_length_high++;
			if (_length_high == 0) {
				/* Message is too long */
				corrupted = 1;
			}
		}

		if (_block_index == 64) {
			_process();
		}

		src++;
	}

}

void SHA1Hasher::finalize(byte_type* dest) {
	if (corrupted) {
		return;
	}

	if (!computed) {
		_pad();
		for (int i = 0; i < 64; ++i) {
			/* message may be sensitive, clear it out */
			_block[i] = 0;
		}
		_length_low = 0; /* and clear length */
		_length_high = 0;
		computed = 1;
	}

	for (int i = 0; i < SHA1_SIZE; ++i) {
		dest[i] = _hash[i >> 2] >> 8 * (3 - (i & 0x03));
	}

}

SHA1Hasher::size_type SHA1Hasher::digest_size() const {
	return SHA1_SIZE;
}

void SHA1Hasher::init() {
	_length_low = 0;
	_length_high = 0;
	_block_index = 0;
	_hash[0] = 0x67452301;
	_hash[1] = 0xEFCDAB89;
	_hash[2] = 0x98BADCFE;
	_hash[3] = 0x10325476;
	_hash[4] = 0xC3D2E1F0;
	computed = 0;
	corrupted = 0;
}

