/*
 * utils.h
 *
 *  Created on: 18/09/2009
 *      Author: alanlohse
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <cstring>
#include <locale>

namespace utils {

template <typename _Tp, typename _Iterator>
inline void clear_it(_Iterator array, const _Tp& value, size_t length) {
	for (; length; array++,length--) {
		*array = value;
	}
}

template <typename _Iterator, typename _ConstIterator>
inline void copy_it(_Iterator dest, _ConstIterator source, size_t length) {
	for (; length; *(dest++) = *(source++),length--) ;
}

template <typename _Iterator, typename _ConstIterator>
inline void r_copy_it(_Iterator dest, _ConstIterator source, size_t length) {
	dest+=length;
	source+=length;
	for (; length; *(--dest) = *(--source),length--) ;
}

template <typename _Iterator>
inline int compare_it(_Iterator s1, _Iterator s2, size_t length) {
	for (; length; s1++, s2++,length--) {
		if (*s1 > *s2)
			return 1;
		if (*s1 < *s2)
			return 1;
	}
	return 0;
}

template <typename _Tp>
inline void swap(_Tp& __1, _Tp& __2) {
	const _Tp __o = __1;
	__1 = __2;
	__2 = __o;
}

template <typename _Tp>
inline _Tp* construct(void* __p) {
	return new (__p) _Tp();
}

template <typename function_T, typename iterator_T>
inline void for_each(iterator_T begin, iterator_T end, function_T func) {
	for (; begin != end; begin++) {
		func(*begin);
	}
}

/**
 * Different from the std::binary_search, it always returns a position,
 * if the value has not been found, an insertion index is returned.
 */
template <typename _Tp, typename iterator_T, class compare_T>
inline iterator_T binary_search(iterator_T _begin, iterator_T _end, const _Tp& _vl, compare_T compare) {
	iterator_T _first = _begin;
	iterator_T _last = _end;
	while (_first < _last) {
	    int _midp = (_last - _first) / 2;
	    iterator_T _midi =  _first + _midp;
	    int cmp = compare(*_midi,_vl);
	    if (cmp < 0)
	    	_first = _midi + 1;
	    else if (cmp > 0)
	    	_last = _midi - 1;
	    else
	    	return _midi;
	}
	return _first;
}

template <typename T>
struct equal_to_nocase : public std::binary_function<T, T, bool> {
	bool operator () (T v1, T v2) const {
		std::locale loc;
		for (; *v1 != 0 && *v2 != 0;) {
			if (std::toupper<T>(*(v1++),loc) != std::toupper<T>(*(v2++),loc)) return false;
		}
		return std::toupper<T>(*v1,loc) == std::toupper<T>(*v2,loc);
	}
};

template <>
struct equal_to_nocase<std::string&> {
	bool operator () (std::string& s1, std::string& s2) const {
		std::locale loc;
		std::string::const_iterator v1 = s1.begin();
		std::string::const_iterator v2 = s2.begin();
		for (; v1 != s1.end() && v2 != s2.end();) {
			if (std::toupper<char>(*(v1++),loc) != std::toupper<char>(*(v2++),loc)) return false;
		}
		return v1 == s1.end() && v2 == s2.end();
	}
};

template <>
struct equal_to_nocase<const std::string&> {
	bool operator () (const std::string& s1, const std::string& s2) const {
		std::locale loc;
		std::string::const_iterator v1 = s1.begin();
		std::string::const_iterator v2 = s2.begin();
		for (; v1 != s1.end() && v2 != s2.end();) {
			if (std::toupper<char>(*(v1++),loc) != std::toupper<char>(*(v2++),loc)) return false;
		}
		return v1 == s1.end() && v2 == s2.end();
	}
};


template <typename T>
struct compare_func: public std::binary_function<T, T, int> {
	int operator () (T v1, T v2) const {
		return v1-v2;
	}
};

template <>
struct compare_func<const std::string&> {
	int operator () (const std::string& v1, const std::string& v2) const {
		return v1.compare(v2);
	}
};

template <>
struct compare_func<const char*> {
	int operator () (const char* v1, const char* v2) const {
		return strcmp(v1,v2);
	}
};

template <>
struct compare_func<char*> {
	int operator () (char* v1, char* v2) const {
		return strcmp(v1,v2);
	}
};

template <>
struct compare_func<const wchar_t*> {
	int operator () (const wchar_t* v1, const wchar_t* v2) const {
		return wcscmp(v1,v2);
	}
};

template <>
struct compare_func<wchar_t*> {
	int operator () (wchar_t* v1, wchar_t* v2) const {
		return wcscmp(v1,v2);
	}
};

template <>
struct compare_func<const float> {
	int operator () (const float& v1, const float& v2) const {
		return v1 == v2 ? 0 : (v1 < v2 ? -1 : 1);
	}
};

template <>
struct compare_func<const double> {
	int operator () (const double& v1, const double& v2) const {
		return v1 == v2 ? 0 : (v1 < v2 ? -1 : 1);
	}
};

template <>
struct compare_func<const long double> {
	int operator () (const long double& v1, const long double& v2) const {
		return v1 == v2 ? 0 : (v1 < v2 ? -1 : 1);
	}
};

template <typename T>
struct hash : public std::unary_function<T, int>{
	int operator () (T v) const {
		return 0;
	}
};

template <>
struct hash<int> {
	int operator () (int v) const {
		return v;
	}
};

template <>
struct hash<char*> {
	int operator () (char* v) const {
		int r = 0;
		for (; *v != 0; v++) r = r*32 + *v - 32;
		return r;
	}
};

template <>
struct hash<const char*> {
	int operator () (const char* v) const {
		int r = 0;
		for (; *v != 0; v++) r = r*32 + *v - 32;
		return r;
	}
};

template <>
struct hash<wchar_t*> {
	int operator () (wchar_t* v) const {
		int r = 0;
		for (; *v != 0; v++) r = r*32 + *v - 32;
		return r;
	}
};

template <>
struct hash<const wchar_t*> {
	int operator () (const wchar_t* v) const {
		int r = 0;
		for (; *v != 0; v++) r = r*32 + *v - 32;
		return r;
	}
};

template <>
struct hash<std::string&> {
	int operator () (std::string& v) const {
		int r = 0;
		std::string::iterator it = v.begin();
		for (; it != v.end(); it++) r = r*32 + *it - 32;
		return r;
	}
};

template <>
struct hash<const std::string&> {
	int operator () (const std::string& v) const {
		int r = 0;
		std::string::const_iterator it = v.begin();
		for (; it != v.end(); it++) r = r*32 + *it - 32;
		return r;
	}
};

template <typename K, typename V>
struct extract_key {
	K operator () (V v) const {
		return K(0);
	}
};

template <typename K, typename V>
struct extract_key<K, std::pair<K,V> > {
	K operator () (const std::pair<K,V>& _v) const {
		return _v.first;
	}
};

enum { _utils_num_primes = 30 };

static const unsigned long _utils_primes[_utils_num_primes] = { 37ul, 71ul, 137ul, 263ul, 503ul, 967ul, 1847ul, 3511ul,
		6673ul, 12689ul, 24113ul, 45817ul, 87071ul, 165437ul, 314339ul, 597253ul, 1134781ul,
		2156087ul, 4096579ul, 7783507ul, 14788679ul, 28098509ul, 53387171ul, 101435629ul,
		192727699ul, 366182647ul, 695747053ul, 1321919449ul, 2147483629ul, 4294967291ul };


inline unsigned long next_prime(unsigned long vl) {
	const unsigned long *_primes = _utils_primes;
	const unsigned long *_last = _utils_primes+(int)_utils_num_primes;
	for (; _primes < _last; _primes++) {
		if (*_primes >= vl) return *_primes;
	}
	return 0;
}

inline unsigned long next_base2(unsigned long vl) {
	unsigned long r = 2;
	while (r < vl) r <<= 1;
	return r;
}

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

struct null_mutex {
	typedef void* native_handle_type;
	void lock(){}
	void try_lock(){}
	void unlock(){}
	native_handle_type native_handle() { return NULL; }
};

template <typename _Type, class _Predicate>
void sync(_Type& ref, _Predicate& pred) {
	ref.mutex.lock();
	try {
		pred(ref);
	} catch(...) {
		ref.mutex.unlock();
		throw;
	}
	ref.mutex.unlock();
}

#define SYNC(obj,block) (obj).mutex.lock(); \
	try { \
	block \
	} catch(...) { \
		(obj).mutex.unlock(); \
		throw; \
	} \
	(obj).mutex.unlock();


}

#endif /* UTILS_H_ */
