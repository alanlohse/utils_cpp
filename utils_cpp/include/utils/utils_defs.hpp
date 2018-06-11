/*
 * utils_defs.hpp
 *
 *  Created on: 08/07/2010
 *      Author: alan.lohse
 */

#ifndef UTILS_DEFS_HPP_
#define UTILS_DEFS_HPP_

#include <utils/config.hpp>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cwchar>
#include <stddef.h>
#include <stdint.h>

typedef signed char			t_small;
typedef unsigned char		t_byte;
typedef short				t_short;
typedef long int			t_int;

typedef unsigned short		t_word;
typedef unsigned long int	t_dword;
typedef unsigned int		t_uint;

typedef unsigned short		u_short;
typedef unsigned long 		u_long;

#ifdef __int64
typedef __int64				t_bigint;
typedef unsigned __int64	t_qword;
#else
typedef long long int		t_bigint;
typedef unsigned long long int t_qword;
#endif

typedef float				t_float;
typedef double 				t_double;

typedef void*				t_pointer;

#ifdef wchar_t
typedef wchar_t				t_wchar;
#else
typedef unsigned short		t_wchar;
#endif

typedef char				t_char;

typedef size_t				t_size;

// VALUES FOR t_result
#define R_OK 0
#define R_ERROR_UNKNOWN -1

typedef long int			t_result;

// allocator configurations
#include <memory>
#define DEFAULT_ALLOCATOR std::allocator

#define MAKE_ENUMERATION(name) private: \
	long value; \
	name() : \
			value(0) { \
	} \
	name(long _value) : \
			value(_value) { \
	} \
public: \
	name(const name& o) : \
			value(o.value) { \
	} \
	~name() { \
	} \
	name& operator = (const name& o) { \
		value = o.value; \
		return *this; \
	} \
	bool operator == (const name& o) const { \
		return value == o.value; \
	} \
	bool operator != (const name& o) const { \
		return value != o.value; \
	} \
	operator long () const { \
		return value; \
	} \
	long ordinal () const { \
		return value; \
	}

#define MAKE_CONSTANT(type,name,init_value) static constexpr type name = init_value
#define MAKE_ENUMERATION2(name) MAKE_ENUMERATION(name) \
	static const name


#define DECLARE_CONSTANT(type,name) static const type name

#define INIT_CONSTANT(type,name,init_value) const type type::name = init_value

#endif /* UTILS_DEFS_HPP_ */
