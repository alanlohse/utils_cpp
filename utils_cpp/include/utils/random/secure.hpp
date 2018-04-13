/*
 * secure.hpp
 *
 *  Created on: 04/07/2015
 *      Author: Alan
 */

#ifndef INCLUDE_UTILS_RANDOM_SECURE_HPP_
#define INCLUDE_UTILS_RANDOM_SECURE_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <limits>

namespace utils {

namespace random {

template <typename _UIntType, class _Hasher, size_t _HashSize, class _Sseq>
class secure_random {
public:

	typedef _UIntType result_type;
	typedef size_t size_type;
	typedef _Hasher hasher_type;
	typedef _Sseq sequencer_type;

	MAKE_CONSTANT(size_type,hash_size,_HashSize);
	MAKE_CONSTANT(result_type,default_seed,1u);
	static const result_type min() { return std::numeric_limits<result_type>::min(); }
	static const result_type max() { return std::numeric_limits<result_type>::max(); }

private:
	typedef typename _Hasher::byte_type byte_type;

	byte_type _cache[_HashSize];
	size_type _pos;
	hasher_type _hasher;
	sequencer_type _seq;

	void _fetch() {
		byte_type _key[_HashSize];
		_hasher.finalize(_cache);
		_seq.generate(_key,_key+_HashSize);
		_hasher.update(_key,_HashSize);
		_pos = 0;
	}

public:

	explicit secure_random(result_type val = default_seed) : _pos(0), _hasher(), _seq() {
		seed(val);
		_fetch();
	}

	template <class Sseq>
	explicit secure_random(Sseq& q) : _pos(0), _hasher(), _seq() {
		seed(q);
		_fetch();
	}

	void seed (result_type val = default_seed) {
		byte_type _key[_HashSize - sizeof(result_type)];
		_hasher.init();
		_hasher.finalize(_cache);
		_seq.generate(_key,_key+(_HashSize-sizeof(result_type)));
		_hasher.update((byte_type*)&val,sizeof(result_type));
		_hasher.update(_key,_HashSize-sizeof(result_type));
	}

	template <class Sseq>
	void seed (Sseq& q) {
		byte_type _key[_HashSize];
		_seq.generate(_key,_key+_HashSize);
		_hasher.updade(_key,_HashSize);
	}


	result_type operator()() {
		if (_HashSize - _pos < sizeof(result_type))
			_fetch();
		result_type r = *((result_type*) (_cache + _pos));
		_pos += sizeof(result_type);
		return r;
	}


};




}  // namespace random

}  // namespace utils


#endif /* INCLUDE_UTILS_RANDOM_SECURE_HPP_ */
