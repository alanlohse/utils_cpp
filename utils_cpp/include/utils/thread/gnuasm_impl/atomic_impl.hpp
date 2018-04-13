/*
 * atomic_impl.hpp
 *
 *  Created on: 13/02/2014
 *      Author: Alan
 */

#ifndef ATOMIC_IMPL_HPP_
#define ATOMIC_IMPL_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/thread/mutex.hpp>

namespace utils {

namespace __gnuasm_impl {

template <typename _Int>
class atomic_base {
public:
	typedef _Int int_type;
protected:
	volatile int_type M_value;
public:
	atomic_base() : M_value(0){}
	atomic_base(const atomic_base& other) : M_value(other.M_value){}

	atomic_base& increase() {
		++M_value;
		return *this;
	}

	atomic_base& decrease() {
		--M_value;
		return *this;
	}

	atomic_base& assign_or(int_type n) {
		M_value |= n;
		return *this;
	}

	atomic_base& assign_and(int_type n) {
		M_value &= n;
		return *this;
	}

	atomic_base& assign_add(int_type n) {
		M_value += n;
		return *this;
	}

	atomic_base& assign_sub(int_type n) {
		M_value -= n;
		return *this;
	}

	atomic_base& assign_xor(int_type n) {
		M_value ^= n;
		return *this;
	}

	int_type get() const {
		return M_value;
	}

	operator int_type() const {
		return M_value;
	}

	atomic_base& set(int_type value) {
		M_value = value;
		return *this;
	}

	int compare(int_type i) {
		return M_value - i;
	}

	atomic_base& operator ++() {
		return increase();
	}

	atomic_base& operator --() {
		return decrease();
	}

	atomic_base operator ++(int) {
		atomic_base tmp(*this);
		increase();
		return tmp;
	}

	atomic_base operator --(int) {
		atomic_base tmp(*this);
		decrease();
		return tmp;
	}

	atomic_base& operator |=(int_type n) {
		return assign_or(n);
	}

	atomic_base& operator &=(int_type n) {
		return assign_and(n);
	}

	atomic_base& operator +=(int_type n) {
		return assign_add(n);
	}

	atomic_base& operator -=(int_type n) {
		return assign_sub(n);
	}

	atomic_base& operator ^=(int_type n) {
		return assign_xor(n);
	}

};

template <>
class atomic_base<long> {
public:
	typedef long int_type;
protected:
	volatile int_type M_value;
	int_type __locked_inc(int_type volatile *addend) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; incq %0"
				: "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; incl %0"
				: "+m" (*addend)
				: : "memory");
#endif
		return *addend;
	}
	int_type __locked_dec(int_type volatile *addend) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; decq %0"
				: "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; decl %0"
				: "+m" (*addend)
				: : "memory");
#endif
		return *addend;
	}
	int_type __locked_add(int_type volatile *addend, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; addq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; addl %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#endif
		return value;
	}
	int_type __locked_sub(int_type volatile *addend, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; subq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; subl %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#endif
		return value;
	}
	int_type __locked_or(int_type volatile *addend, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; orq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; orl %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#endif
		return value;
	}
	int_type __locked_xor(int_type volatile *addend, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; xorq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; xorl %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#endif
		return value;
	}
	int_type __locked_and(int_type volatile *addend, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; andq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#else
		__asm__ __volatile__ ("lock ; andl %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
#endif
		return value;
	}
	int_type __locked_exchange(int_type volatile *target, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; xchgq %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
#else
		__asm__ __volatile__ ("lock ; xchgl %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
#endif
		return value;
	}
	int __locked_compare(int_type volatile *target, int_type value) {
#ifdef __x86_64__
		__asm__ __volatile__ ("lock ; cmpq %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
#else
		__asm__ __volatile__ ("lock ; cmpl %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
#endif
		return value;
	}
	int_type __locked_get(const int_type volatile *target) const {
		return *target;
	}
public:

	atomic_base() {
		set(0);
	}
	atomic_base(const atomic_base& other) {
		set(other.get());
	}

	atomic_base& increase() {
		__locked_inc(&M_value);
		return *this;
	}

	atomic_base& decrease() {
		__locked_dec(&M_value);
		return *this;
	}

	atomic_base& assign_or(int_type n) {
		__locked_or(&M_value, n);
		return *this;
	}

	atomic_base& assign_and(int_type n) {
		__locked_and(&M_value, n);
		return *this;
	}

	atomic_base& assign_add(int_type n) {
		__locked_add(&M_value, n);
		return *this;
	}

	atomic_base& assign_sub(int_type n) {
		__locked_sub(&M_value, n);
		return *this;
	}

	atomic_base& assign_xor(int_type n) {
		__locked_xor(&M_value, n);
		return *this;
	}

	operator int_type() const {
		return get();
	}

	int_type get() const {
		return __locked_get(&M_value);
	}

	int_type set(int_type value) {
		return __locked_exchange(&M_value, value);
	}

	int compare(int_type i) {
		return __locked_compare(&M_value,i);
	}

	atomic_base& operator ++() {
		return increase();
	}

	atomic_base& operator --() {
		return decrease();
	}

	atomic_base& operator |=(int_type n) {
		return assign_or(n);
	}

	atomic_base& operator &=(int_type n) {
		return assign_and(n);
	}

	atomic_base& operator +=(int_type n) {
		return assign_add(n);
	}

	atomic_base& operator -=(int_type n) {
		return assign_sub(n);
	}

	atomic_base& operator ^=(int_type n) {
		return assign_xor(n);
	}

};

template <>
class atomic_base<long long> {
public:
	typedef long long int_type;
protected:
	volatile int_type M_value;
#ifdef __x86_64__
	int_type __locked_inc(int_type volatile *addend) {
		__asm__ __volatile__ ("lock incq %0"
				: "+m" (*addend)
				: : "memory");
		return *addend;
	}
	int_type __locked_dec(int_type volatile *addend) {
		__asm__ __volatile__ ("lock decq %0"
				: "+m" (*addend)
				: : "memory");
		return *addend;
	}
	int_type __locked_add(int_type volatile *addend, int_type value) {
		__asm__ __volatile__ ("lock addq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
		return value;
	}
	int_type __locked_sub(int_type volatile *addend, int_type value) {
		__asm__ __volatile__ ("lock subq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
		return value;
	}
	int_type __locked_or(int_type volatile *addend, int_type value) {
		__asm__ __volatile__ ("lock orq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
		return value;
	}
	int_type __locked_xor(int_type volatile *addend, int_type value) {
		__asm__ __volatile__ ("lock xorq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
		return value;
	}
	int_type __locked_and(int_type volatile *addend, int_type value) {
		__asm__ __volatile__ ("lock andq %0,%1"
				: "+r" (value), "+m" (*addend)
				: : "memory");
		return value;
	}
	int_type __locked_exchange(int_type volatile *target, int_type value) {
		__asm__ __volatile__ ("lock ; xchgq %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
		return value;
	}
	int_type __locked_compare(int_type volatile *target, int_type value) {
		__asm__ __volatile__ ("lock ; cmpq %0,%1"
				: "=r"(value)
				: "m"(*target),"0"(value)
				: "memory");
		return value;
	}
	int_type __locked_get(const int_type volatile *target) const {
		return *target;
	}
#else
	Mutex mutex;
	int_type __locked_inc(int_type volatile *addend) {
		mutex.lock();
		++(*addend);
		mutex.unlock();
		return *addend;
	}
	int_type __locked_dec(int_type volatile *addend) {
		mutex.lock();
		--(*addend);
		mutex.unlock();
		return *addend;
	}
	int_type __locked_add(int_type volatile *addend, int_type value) {
		mutex.lock();
		*addend += value;
		mutex.unlock();
		return *addend;
	}
	int_type __locked_or(int_type volatile *addend, int_type value) {
		mutex.lock();
		*addend |= value;
		mutex.unlock();
		return *addend;
	}
	int_type __locked_and(int_type volatile *addend, int_type value) {
		mutex.lock();
		*addend &= value;
		mutex.unlock();
		return *addend;
	}
	int_type __locked_xor(int_type volatile *addend, int_type value) {
		mutex.lock();
		*addend ^= value;
		mutex.unlock();
		return *addend;
	}
	int_type __locked_sub(int_type volatile *addend, int_type value) {
		mutex.lock();
		*addend -= value;
		mutex.unlock();
		return *addend;
	}
	int_type __locked_get(const int_type volatile *addend) const {
		return *addend;
	}
	int_type __locked_exchange(int_type volatile *target, int_type value) {
		mutex.lock();
		int_type tmp = *target;
		*target = value;
		value = tmp;
		mutex.unlock();
		return value;
	}
	int __locked_compare(int_type volatile *target, int_type value) {
		mutex.lock();
		value = *target - value;
		mutex.unlock();
		return value;
	}
#endif
public:
	atomic_base& increase() {
		__locked_inc(&M_value);
		return *this;
	}

	atomic_base& decrease() {
		__locked_dec(&M_value);
		return *this;
	}

	atomic_base& assign_or(int_type n) {
		__locked_or(&M_value, n);
		return *this;
	}

	atomic_base& assign_and(int_type n) {
		__locked_and(&M_value, n);
		return *this;
	}

	atomic_base& assign_add(int_type n) {
		__locked_add(&M_value, n);
		return *this;
	}

	atomic_base& assign_sub(int_type n) {
		__locked_sub(&M_value, n);
		return *this;
	}

	atomic_base& assign_xor(int_type n) {
		__locked_xor(&M_value, n);
		return *this;
	}

	operator int_type() const {
		return get();
	}

	int_type get() const {
		return __locked_get(&M_value);
	}

	int_type set(int_type value) {
		return __locked_exchange(&M_value, value);
	}

	int compare(int_type i) {
		return __locked_compare(&M_value,i);
	}

	atomic_base& operator ++() {
		return increase();
	}

	atomic_base& operator --() {
		return decrease();
	}

	atomic_base& operator |=(int_type n) {
		return assign_or(n);
	}

	atomic_base& operator &=(int_type n) {
		return assign_and(n);
	}

	atomic_base& operator +=(int_type n) {
		return assign_add(n);
	}

	atomic_base& operator -=(int_type n) {
		return assign_sub(n);
	}

	atomic_base& operator ^=(int_type n) {
		return assign_xor(n);
	}

};


template<typename _Int>
struct atomic_base_type {
	typedef _Int int_type;
	typedef _Int base_int_type;
};

#define DEFINE_ATOMIC_BASE_TYPE(intTp,baseTp) template<> \
struct atomic_base_type<intTp> { \
	typedef intTp int_type; \
	typedef baseTp base_int_type; \
};

DEFINE_ATOMIC_BASE_TYPE(int,long)
DEFINE_ATOMIC_BASE_TYPE(long,long)
DEFINE_ATOMIC_BASE_TYPE(unsigned int,long)
DEFINE_ATOMIC_BASE_TYPE(unsigned long,long)

DEFINE_ATOMIC_BASE_TYPE(long long,long long)
DEFINE_ATOMIC_BASE_TYPE(unsigned long long,long long)

} // namespace __win32_impl

} // namespace utils

#endif /* ATOMIC_IMPL_HPP_ */
