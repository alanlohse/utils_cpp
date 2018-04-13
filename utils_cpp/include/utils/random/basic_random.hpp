/*
 * basic_random.hpp
 *
 *  Created on: 03/07/2015
 *      Author: Alan
 */

#ifndef INCLUDE_UTILS_RANDOM_BASIC_RANDOM_HPP_
#define INCLUDE_UTILS_RANDOM_BASIC_RANDOM_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <vector>

namespace utils {

namespace random {

class seed_seq {
public:
	/** The type of the seed vales. */
	typedef uint_least32_t result_type;

private:
	std::vector<result_type> _v;

	template <typename _Type>
	_Type func_T(_Type x) {
		return x ^ (x >> 27);
	}
public:

	/** Default constructor. */
	seed_seq() :
			_v() {
	}

	template <typename _IntType, size_t _N>
	seed_seq(const _IntType(& il)[_N]) :_v() {
		const _IntType *it = il;
		const _IntType *ed = il + _N;
		for (; it != ed; ++it)
			_v.push_back(*it);
	}

	template <typename _InputIterator>
	seed_seq(_InputIterator begin, _InputIterator end) {
		for (_InputIterator it = begin; it != end; ++it)
			_v.push_back(*it);
	}

	template <typename _RandomAccessIterator>
	void generate(_RandomAccessIterator begin, _RandomAccessIterator end) {
		if (begin == end)
			return;

		 typedef typename std::iterator_traits<_RandomAccessIterator>::value_type
		        _Type;

		std::fill(begin, end, _Type(0x8b8b8b8bu));

		const size_t n = end - begin;
		const size_t s = _v.size();
		const size_t t = (n >= 623) ? 11 :
						 (n >= 68) ? 7 :
						 (n >= 39) ? 5 :
						 (n >= 7) ? 3 :
						 (n - 1) / 2;
		const size_t p = (n - t) / 2;
		const size_t q = p + t;
		const size_t m = std::max(size_t(s + 1), n);

		for (size_t k = 0; k < m; ++k) {
			_Type r1 = 1664525u * func_T(begin[k % n] ^ begin[(k + p) % n] ^ begin[(k - 1) % n]);
			_Type r2 = r1;
			if (k == 0)
				r2 += s;
			else if (k <= s)
				r2 += k % n + _v[k - 1];
			else
				r2 += k % n;
			begin[(k + p) % n] += r1;
			begin[(k + q) % n] += r2;
			begin[k % n] = r2;
		}

		for (size_t k = m; k < m + n; ++k) {
			_Type r3 = 1566083941u * func_T(begin[k % n] + begin[(k + p) % n] + begin[(k - 1) % n]);
			_Type r4 = r3 - k % n;
			begin[(k + p) % n] ^= r3;
			begin[(k + q) % n] ^= r4;
			begin[k % n] = r4;
		}
	}

	size_t size() const {
		return _v.size();
	}

	template <typename OutputIterator>
	void param(OutputIterator dest) const {
		std::copy(_v.begin(), _v.end(), dest);
	}

};

template <class _UIntType, _UIntType _a, _UIntType _c, _UIntType _m>
class linear_congruential_engine {
public:
	typedef _UIntType result_type;

	/** The multiplier. */
	MAKE_CONSTANT(result_type,multiplier,_a);
	/** An increment. */
	MAKE_CONSTANT(result_type,increment,_c);
	/** The modulus. */
	MAKE_CONSTANT(result_type,modulus,_m);MAKE_CONSTANT(result_type,default_seed,1u);
	static const result_type min() { return _c == 0u ? 1u: 0u; }
	static const result_type max() { return _m - 1u; }

private:
	_UIntType _seed;

public:

	explicit linear_congruential_engine(result_type val = default_seed) {
		seed(val);
	}

	template <class Sseq>
	explicit linear_congruential_engine(Sseq& q) {
		seed(q);
	}

	void seed (result_type val = default_seed) {

	}

	template <class Sseq>
	void seed (Sseq& q) {

	}


	result_type operator()() { return _seed; }

};

}  // namespace random

}  // namespace utils

#endif /* INCLUDE_UTILS_RANDOM_BASIC_RANDOM_HPP_ */
