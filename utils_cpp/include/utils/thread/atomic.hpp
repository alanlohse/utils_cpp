/*
 * atomic.hpp
 *
 *  Created on: 13/02/2014
 *      Author: Alan
 */

#ifndef ATOMIC_HPP_
#define ATOMIC_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>

#include <utils/thread/gnuasm_impl/atomic_impl.hpp>

namespace utils {

template<typename _Int>
class atomic {
public:
	typedef _Int int_type;
private:
	typedef typename __gnuasm_impl::atomic_base_type<_Int>::base_int_type base_int_type;
	__gnuasm_impl::atomic_base<base_int_type> M_impl;
public:
	atomic() {
		set(0);
	}

	explicit atomic(int_type value) {
		set(value);
	}

	explicit atomic(atomic& other) {
		set(other.get());
	}

	atomic& increase() {
		M_impl.increase();
		return *this;
	}

	atomic& decrease() {
		M_impl.decrease();
		return *this;
	}

	atomic& assign_or(int_type n) {
		M_impl.assign_or(n);
		return *this;
	}

	atomic& assign_and(int_type n) {
		M_impl.assign_and(n);
		return *this;
	}

	atomic& assign_add(int_type n) {
		M_impl.assign_add(n);
		return *this;
	}

	atomic& assign_sub(int_type n) {
		M_impl.assign_sub(n);
		return *this;
	}

	atomic& assign_xor(int_type n) {
		M_impl.assign_xor(n);
		return *this;
	}

	operator int_type() const {
		return get();
	}

	int_type get() const {
		return M_impl.get();
	}

	int_type set(int_type value) {
		return M_impl.set(value);
	}

	int compare(int_type i) {
		return M_impl.compare(i);
	}

	atomic& operator ++() {
		return increase();
	}

	atomic& operator --() {
		return decrease();
	}

	atomic operator ++(int) {
		atomic tmp(*this);
		++ *this;
		return tmp;
	}

	atomic operator --(int) {
		atomic tmp(*this);
		-- *this;
		return tmp;
	}

	atomic& operator |=(int_type n) {
		return assign_or(n);
	}

	atomic& operator &=(int_type n) {
		return assign_and(n);
	}

	atomic& operator +=(int_type n) {
		return assign_add(n);
	}

	atomic& operator -=(int_type n) {
		return assign_sub(n);
	}

	atomic& operator ^=(int_type n) {
		return assign_xor(n);
	}

	atomic& operator = (int_type value) {
		set(value);
		return *this;
	}

	atomic& operator = (atomic& other) {
		set(other.get());
		return *this;
	}

	bool operator == (const atomic& other) {
		return compare(other.get()) == 0;
	}

	bool operator == (int_type other) {
		return compare(other) == 0;
	}

	bool operator > (const atomic& other) {
		return compare(other.get()) > 0;
	}

	bool operator > (int_type other) {
		return compare(other) > 0;
	}

	bool operator < (const atomic& other) {
		return compare(other.get()) < 0;
	}

	bool operator < (int_type other) {
		return compare(other) < 0;
	}

	bool operator >= (const atomic& other) {
		return compare(other.get()) >= 0;
	}

	bool operator >= (int_type other) {
		return compare(other) >= 0;
	}

	bool operator <= (const atomic& other) {
		return compare(other.get()) <= 0;
	}

	bool operator <= (int_type other) {
		return compare(other) <= 0;
	}

	bool operator != (const atomic& other) {
		return compare(other.get()) != 0;
	}

	bool operator != (int_type other) {
		return compare(other) != 0;
	}

};

}


#endif /* ATOMIC_HPP_ */
