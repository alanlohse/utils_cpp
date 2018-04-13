/*
 * iterators.hpp
 *
 *  Created on: 16/01/2013
 *      Author: Alan
 */

#ifndef ITERATORS_HPP_
#define ITERATORS_HPP_


#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/exception.hpp>
#include <new>


namespace utils {

namespace iterators {

template <typename _Tp>
struct pointer_iterator {
	typedef pointer_iterator 									iterator;
    typedef typename
		std::iterator_traits<_Tp*>::iterator_category		iterator_category;
    typedef typename
		std::iterator_traits<_Tp*>::value_type  			value_type;
    typedef typename
		std::iterator_traits<_Tp*>::difference_type			difference_type;
    typedef typename std::iterator_traits<_Tp*>::reference	reference;
    typedef typename std::iterator_traits<_Tp*>::pointer	pointer;
	_Tp* _M_begin;
	_Tp* _M_cur;
	_Tp* _M_end;
	pointer_iterator(_Tp* _cur, _Tp* _begin, _Tp* _end) {
		_M_cur = _cur;
		_M_begin = _begin;
		_M_end = _end;
	}
	reference operator*() const {
		if (_M_cur >= _M_end || _M_cur < _M_begin)
			throw boundary_exceeded_exception();
		return *_M_cur;
	}
	pointer operator->() const {
		return _M_cur;
	}
	iterator& operator++() {
		_M_cur++;
		if (_M_cur > _M_end || _M_cur < _M_begin)
			throw boundary_exceeded_exception();
		return *this;
	}
	iterator operator++(int) {
		iterator __tmp = *this;
		++*this;
		return __tmp;
	}
	iterator& operator--() {
		_M_cur--;
		if (_M_cur > _M_end || _M_cur < _M_begin)
			throw boundary_exceeded_exception();
		return *this;
	}
	iterator operator--(int) {
		iterator __tmp = *this;
		--*this;
		return __tmp;
	}
	iterator& operator+=(difference_type n) {
		_M_cur += n;
		if (_M_cur > _M_end || _M_cur < _M_begin)
			throw boundary_exceeded_exception();
		return *this;
	}
	iterator& operator-=(difference_type n) {
		_M_cur -= n;
		if (_M_cur > _M_end || _M_cur < _M_begin)
			throw boundary_exceeded_exception();
		return *this;
	}
	iterator operator-(difference_type n) {
		iterator __tmp = *this;
		__tmp -= n;
		return __tmp;
	}
	iterator operator+(difference_type n) {
		iterator __tmp = *this;
		__tmp += n;
		return __tmp;
	}
	bool operator==(const iterator& __it) const {
		return _M_cur == __it._M_cur;
	}
	bool operator!=(const iterator& __it) const {
		return _M_cur != __it._M_cur;
	}
	difference_type operator-(const iterator& __it) const {
		return _M_cur-__it._M_cur;
	}

};

template <typename _Tp, typename _Get_Next>
struct forward_list_iterator {
	typedef forward_list_iterator 									iterator;
    typedef typename
		std::iterator_traits<_Tp*>::iterator_category		iterator_category;
    typedef typename
		std::iterator_traits<_Tp*>::value_type  			value_type;
    typedef typename
		std::iterator_traits<_Tp*>::difference_type			difference_type;
    typedef typename std::iterator_traits<_Tp*>::reference	reference;
    typedef typename std::iterator_traits<_Tp*>::pointer	pointer;
	_Tp* _M_cur;
	_Get_Next next;
	forward_list_iterator(_Tp* _begin) {
		_M_cur = _begin;
	}
	reference operator*() const {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		return *_M_cur;
	}
	pointer operator->() const {
		return _M_cur;
	}
	iterator& operator++() {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		_M_cur = next(_M_cur);
		return *this;
	}
	iterator operator++(int) {
		iterator __tmp = *this;
		++*this;
		return __tmp;
	}
	iterator& operator+=(difference_type n) {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		for (; n >= 0; --n)
			_M_cur = next(_M_cur);
		return *this;
	}
	iterator operator+(difference_type n) {
		iterator __tmp = *this;
		__tmp += n;
		return __tmp;
	}
	bool operator==(const iterator& __it) const {
		return _M_cur == __it._M_cur;
	}
	bool operator!=(const iterator& __it) const {
		return _M_cur != __it._M_cur;
	}
	difference_type operator-(const iterator& __it) const {
		_Tp* cur = _M_cur;
		for (difference_type n = 0; cur; n++, cur = next(cur))
			if (cur == __it._M_cur)
				return n;
		return -1;
	}

};

template <typename _Tp, typename _Get_previous>
struct backward_list_iterator {
	typedef backward_list_iterator 									iterator;
    typedef typename
		std::iterator_traits<_Tp*>::iterator_category		iterator_category;
    typedef typename
		std::iterator_traits<_Tp*>::value_type  			value_type;
    typedef typename
		std::iterator_traits<_Tp*>::difference_type			difference_type;
    typedef typename std::iterator_traits<_Tp*>::reference	reference;
    typedef typename std::iterator_traits<_Tp*>::pointer	pointer;
	_Tp* _M_cur;
	_Get_previous prev;
	backward_list_iterator(_Tp* _begin) {
		_M_cur = _begin;
	}
	reference operator*() const {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		return *_M_cur;
	}
	pointer operator->() const {
		return _M_cur;
	}
	iterator& operator--() {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		_M_cur = prev(_M_cur);
		return *this;
	}
	iterator operator--(int) {
		iterator __tmp = *this;
		++*this;
		return __tmp;
	}
	iterator& operator-=(difference_type n) {
		if (!_M_cur)
			throw boundary_exceeded_exception();
		for (; n >= 0; --n)
			_M_cur = prev(_M_cur);
		return *this;
	}
	iterator operator-(difference_type n) {
		iterator __tmp = *this;
		__tmp -= n;
		return __tmp;
	}
	bool operator==(const iterator& __it) const {
		return _M_cur == __it._M_cur;
	}
	bool operator!=(const iterator& __it) const {
		return _M_cur != __it._M_cur;
	}
	difference_type operator-(const iterator& __it) const {
		_Tp* cur = __it._M_cur;
		for (difference_type n = 0; cur; n++, cur = prev(cur))
			if (cur == _M_cur)
				return n;
		return -1;
	}

};

}

}

#endif /* ITERATORS_HPP_ */
