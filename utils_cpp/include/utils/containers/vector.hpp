#ifndef __UTILS_VECTOR__
#define __UTILS_VECTOR__

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <iterator>
#include <stdexcept>

namespace utils {

namespace containers {
	
template <typename _T>
struct _vector_iterator {
	typedef _T value_type;
	typedef ptrdiff_t difference_type;
	typedef _T* pointer;
	typedef _T& reference;
	typedef std::random_access_iterator_tag iterator_category;
	typedef _vector_iterator iterator;
	pointer _ptr;
	_vector_iterator() : _ptr(0) { }
	_vector_iterator(pointer ptr) : _ptr(ptr) { }
	iterator& operator = (const iterator& other) {
		_ptr = other._ptr;
		return *this;
	}
	pointer operator -> () const {
		return _ptr;
	}
	reference operator * () const {
		return *_ptr;
	}
	iterator& operator ++() {
		++_ptr;
		return *this;
	}
	iterator operator ++(int) {
		iterator tmp = *this;
		++(*this);
		return tmp;
	}
	iterator& operator --() {
		--_ptr;
		return *this;
	}
	iterator operator --(int) {
		iterator tmp = *this;
		--(*this);
		return tmp;
	}
	iterator& operator +=(difference_type n) {
		_ptr+=n;
		return *this;
	}
	iterator& operator -=(difference_type n) {
		_ptr-=n;
		return *this;
	}
	iterator operator + (difference_type n) const {
		iterator tmp = *this;
		tmp+=n;
		return tmp;
	}
	iterator operator - (difference_type n) const {
		iterator tmp = *this;
		tmp-=n;
		return tmp;
	}
	difference_type operator - (const iterator& b) const {
		return _ptr - b._ptr;
	}
	reference operator [](difference_type n) {
		return *(_ptr + n);
	}
};
template <typename _T>
inline bool operator == (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr == b._ptr;
}
template <typename _T>
inline bool operator != (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr != b._ptr;
}
template <typename _T>
inline bool operator > (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr > b._ptr;
}
template <typename _T>
inline bool operator < (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr < b._ptr;
}
template <typename _T>
inline bool operator >= (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr >= b._ptr;
}
template <typename _T>
inline bool operator <= (const _vector_iterator<_T>& a, const _vector_iterator<_T>& b) {
	return a._ptr <= b._ptr;
}
template <typename _T>
inline _vector_iterator<_T> operator + (typename _vector_iterator<_T>::difference_type n, const _vector_iterator<_T>& a) {
	return _vector_iterator<_T>(a + n);
}
template <typename _T>
inline _vector_iterator<_T> operator - (typename _vector_iterator<_T>::difference_type n, const _vector_iterator<_T>& a) {
	return _vector_iterator<_T>(a - n);
}

template <typename _T>
struct _vector_const_iterator {
	typedef _T value_type;
	typedef ptrdiff_t difference_type;
	typedef const _T* pointer;
	typedef const _T& reference;
	typedef std::random_access_iterator_tag iterator_category;
	typedef _vector_const_iterator iterator;
	pointer _ptr;
	_vector_const_iterator() : _ptr(0) { }
	_vector_const_iterator(pointer ptr) : _ptr(ptr) { }
	iterator& operator = (const iterator& other) {
		_ptr = other._ptr;
		return *this;
	}
	pointer operator -> () const {
		return _ptr;
	}
	reference operator * () const {
		return *_ptr;
	}
	iterator& operator ++() {
		++_ptr;
		return *this;
	}
	iterator operator ++(int) {
		iterator tmp = *this;
		++(*this);
		return tmp;
	}
	iterator& operator --() {
		--_ptr;
		return *this;
	}
	iterator operator --(int) {
		iterator tmp = *this;
		--(*this);
		return tmp;
	}
	iterator& operator +=(difference_type n) {
		_ptr+=n;
		return *this;
	}
	iterator& operator -=(difference_type n) {
		_ptr-=n;
		return *this;
	}
	iterator operator + (difference_type n) const {
		iterator tmp = *this;
		tmp+=n;
		return tmp;
	}
	iterator operator - (difference_type n) const {
		iterator tmp = *this;
		tmp-=n;
		return tmp;
	}
	difference_type operator - (const iterator& b) const {
		return _ptr - b._ptr;
	}
	reference operator [](difference_type n) {
		return *(_ptr + n);
	}
};
template <typename _T>
inline bool operator == (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr == b._ptr;
}
template <typename _T>
inline bool operator != (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr != b._ptr;
}
template <typename _T>
inline bool operator > (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr > b._ptr;
}
template <typename _T>
inline bool operator < (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr < b._ptr;
}
template <typename _T>
inline bool operator >= (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr >= b._ptr;
}
template <typename _T>
inline bool operator <= (const _vector_const_iterator<_T>& a, const _vector_const_iterator<_T>& b) {
	return a._ptr <= b._ptr;
}
template <typename _T>
inline _vector_const_iterator<_T> operator + (typename _vector_iterator<_T>::difference_type n, const _vector_const_iterator<_T>& a) {
	return _vector_const_iterator<_T>(a + n);
}
template <typename _T>
inline _vector_const_iterator<_T> operator - (typename _vector_iterator<_T>::difference_type n, const _vector_const_iterator<_T>& a) {
	return _vector_const_iterator<_T>(a - n);
}


template <typename _T, class _Alloc = std::allocator<_T>, class _Mutex = null_mutex>
class vector {
public:
	typedef _T value_type;
	typedef _Alloc allocator_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef _vector_iterator<_T> iterator;
	typedef _vector_const_iterator<_T> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
protected:
	typedef _Mutex mutex_type;
	pointer _begin;
	size_type _count, _maxcount;
	mutex_type mutex;
	allocator_type _allocator;

	void destroy_items(bool dealloc) {
		pointer it = _begin + _count - 1;
		for (; it >= _begin; --it)
			_allocator.destroy(it);
		_count = 0;
		if (dealloc)
			_allocator.deallocate(_begin,_maxcount);
	}

	void move_part(pointer pos, size_type n) {
		pointer cur = pos + n;
		while (cur-- > pos) {
			if (cur + n >= _begin + _count)
				_allocator.construct(cur+n,*cur);
			else
				*(cur+n) = *cur;
			_allocator.destroy(cur);
		}
	}

	void move_tofront(pointer pos, size_type n) {
		pointer cur = pos;
		pointer end = pos + n;
		for (;cur != end; ++cur) {
			if (cur + n < _begin + _count) {
				*cur = *(cur+n);
				_allocator.destroy(cur+n);
			}
		}
	}

public:
	explicit vector(const allocator_type& alloc = allocator_type()) : _begin(0), _count(0),
			_maxcount(0), mutex(), _allocator(alloc) {
		_maxcount = 16;
		_begin = _allocator.allocate(_maxcount);
	}
	explicit vector(size_type n, const value_type& val = value_type(),
                 const allocator_type& alloc = allocator_type()) : _begin(0), _count(n),
			_maxcount(0), mutex(), _allocator(alloc) {
		_maxcount = 16;
		while (_maxcount < n) _maxcount <<= 1;
		_begin = _allocator.allocate(_maxcount);
		pointer cur = _begin;
		pointer en = _begin + _count;
		for (; cur != en; ++cur)
			_allocator.construct(cur,val);
	}
	template <class InputIterator>
         vector(InputIterator first, InputIterator last,
                 const allocator_type& alloc = allocator_type()) : _begin(0), _count(0),
			_maxcount(0), mutex(), _allocator(alloc) {
		_maxcount = 16;
		_begin = _allocator.allocate(_maxcount);
		insert(end(),first,last);
	}
	vector(const vector& x) : _begin(0), _count(x._count),
			_maxcount(x._maxcount), mutex(), _allocator(x._allocator) {
		pointer cur = _begin;
		pointer en = _begin + _count;
		const_iterator it = x.begin();
		_begin = _allocator.allocate(_maxcount);
		for (; cur != en; ++cur, ++it)
			_allocator.construct(cur,*it);
	}
	template <typename _Type, size_t _Count>
	explicit vector(const _T(& _array)[_Count]) : _begin(0), _count(0),
			_maxcount(0), mutex(), _allocator() {
		_maxcount = 16;
		while (_maxcount < _Count) _maxcount <<= 1;
		_begin = _allocator.allocate(_maxcount);
		const _T* first = _array;
		const _T* last = _array + _Count;
		insert(end(),first,last);
	}
	~vector() {
    	mutex.lock();
		destroy_items(true);
    	mutex.unlock();
	}
	
	iterator begin() {
		return iterator(_begin);
	}
	iterator end() {
		return iterator(_begin + _count);
	}
	const_iterator begin() const {
		return const_iterator(_begin);
	}
	const_iterator end() const {
		return const_iterator(_begin + _count);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}

	size_type size() const {
		return _count;
	}
	size_type max_size() const {
		return _allocator.max_size();
	}
	size_type capacity() const {
		return _maxcount;
	}
	bool empty() const {
		return _count == 0;
	}
	void resize(size_type n, value_type val = value_type()) {
    	mutex.lock();
		if (n > _count) {
			if (n > _maxcount) {
				size_type n_maxcount = _maxcount << 1;
				while (n > n_maxcount) n_maxcount <<= 1;
				reserve(n_maxcount);
			}
			while (_count < n)
				push_back(val);
		} else if (n < _count) {
			while (_count > n)
				pop_back();
		}
    	mutex.unlock();
	}
	void reserve(size_type n) {
		if (n > max_size()) throw std::length_error("Required size is greater than the maximum allowed");
		if (n > _maxcount) {
	    	mutex.lock();
			const_iterator it = begin();
			const_iterator en = end();
			pointer new_begin = _allocator.allocate(n);
			pointer cur = new_begin;
			for (; cur != en; ++cur, ++it)
				_allocator.construct(cur,*it);
			destroy_items(true);
			_begin = new_begin;
			_maxcount = n;
	    	mutex.unlock();
		}
	}

    reference operator[] (size_type n) {
    	return *(_begin + n);
    }
    const_reference operator[] (size_type n) const {
    	return *(_begin + n);
    }
    reference at (size_type n) {
    	return *(_begin + n);
    }
    const_reference at (size_type n) const {
    	return *(_begin + n);
    }

    reference front() {
    	return *_begin;
    }
    const_reference front() const {
    	return *_begin;
    }

    reference back() {
    	return *(_begin + _count - 1);
    }
    const_reference back() const {
    	return *(_begin + _count - 1);
    }

    template <class InputIterator>
    	void assign(InputIterator first, InputIterator last) {
    	mutex.lock();
    	destroy_items(false);
		size_type n_maxcount = _maxcount;
		size_type n = last - first;
		while (n_maxcount < n) n_maxcount <<= 1;
		reserve(n_maxcount);
		pointer ptr = _begin;
		pointer endp = _begin + n;
		for (;ptr != endp; ++ptr, ++first)
			_allocator.construct(ptr,*first);
		_count = n;
    	mutex.unlock();
    }
    void assign(size_type n, const value_type& val) {
    	mutex.lock();
    	destroy_items(false);
		size_type n_maxcount = _maxcount;
		while (n_maxcount < n) n_maxcount <<= 1;
		reserve(n_maxcount);
		pointer ptr = _begin;
		pointer endp = _begin + n;
		for (;ptr != endp; ++ptr)
			_allocator.construct(ptr,val);
		_count = n;
    	mutex.unlock();
    }

    void push_back (const value_type& val) {
    	mutex.lock();
    	size_type n_count = _count + 1;
		if (n_count > _maxcount) {
			size_type n_maxcount = _maxcount << 1;
			while (n_count > n_maxcount) n_maxcount <<= 1;
			reserve(n_maxcount);
		}
		_allocator.construct(_begin + (_count++),val);
    	mutex.unlock();
    }

    void pop_back() {
    	mutex.lock();
    	if (_count > 0) {
    		--_count;
    		_allocator.destroy(_begin + _count);
    	}
    	mutex.unlock();
    }

    iterator insert(iterator position, const value_type& val) {
    	mutex.lock();
    	if (position == end()) {
			push_back(val);
	    	mutex.unlock();
			return iterator(_begin + _count - 1);
    	} else {
    		size_type n = position - begin();
    		insert(position, 1, val);
    		mutex.unlock();
			return iterator(_begin + n);
    	}
    }
    void insert(iterator position, size_type n, const value_type& val) {
    	mutex.lock();
    	if (position == end()) {
    		while (n-- > 0)
    			push_back(val);
    	} else {
			if (_count + n > _maxcount) {
				size_type n_maxcount = _maxcount << 1;
				while (_count + n > n_maxcount) n_maxcount <<= 1;
				reserve(n_maxcount);
			}
			move_part(position._ptr,n);
			pointer ptr = position._ptr;
			pointer endp = position._ptr + n;
			for (; ptr < endp; ++ptr)
				_allocator.construct(ptr,val);
    	}
    	mutex.unlock();
    }
    template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last) {
    	mutex.lock();
    	if (position == end()) {
			InputIterator cur = first;
			for (; cur != last; ++cur)
				push_back(*cur);
    	} else {
    		size_type n = last - first;
			if (_count + n > _maxcount) {
				size_type n_maxcount = _maxcount << 1;
				while (_count + n > n_maxcount) n_maxcount <<= 1;
				reserve(n_maxcount);
			}
			move_part(position._ptr,n);
			pointer ptr = position._ptr;
			pointer endp = position._ptr + n;
			for (; ptr < endp; ++ptr, ++first)
				_allocator.construct(ptr,*first);
    	}
    	mutex.unlock();
    }

    vector& operator = (const vector& x) {
    	assign(x.begin(),x.end());
    	return *this;
    }

    iterator erase(iterator position) {
    	if (position == end() - 1) {
    		pop_back();
    		return end();
    	} else {
    		return erase(position, position+1);
    	}
    }
    iterator erase(iterator first, iterator last) {
    	mutex.lock();
    	size_type n = last - first;
    	move_tofront(first._ptr,n);
    	mutex.unlock();
    	return first;
    }

    void swap (vector& x) {
    	mutex.lock();
    	x.mutex.lock();
    	utils::swap(_begin,x._begin);
    	utils::swap(_count,x._count);
    	utils::swap(_maxcount,x._maxcount);
    	utils::swap(_allocator,x._allocator);
    	x.mutex.unlock();
    	mutex.unlock();
    }

    void clear() {
    	mutex.lock();
    	destroy_items(false);
    	mutex.unlock();
    }

    allocator_type get_allocator() const {
    	return _allocator;
    }

	template<class _Predicate>
	void sync(_Predicate pred) {
		mutex.lock();
		try {
			pred(*this);
		} catch(...) {
			mutex.unlock();
			throw;
		}
		mutex.unlock();
	}

	template<class _Predicate>
	void forEach(_Predicate pred) {
		mutex.try_lock();
		try {
			iterator it = begin();
			iterator finis = end();
			for (; it != finis; ++it)
				pred(*it);
		} catch(...) {
			mutex.unlock();
			throw;
		}
		mutex.unlock();
	}

	template<class _Predicate>
	vector filter(_Predicate pred) {
		vector r;
		mutex.try_lock();
		try {
			iterator it = begin();
			iterator finis = end();
			for (; it != finis; ++it)
				if (pred(*it))
					r.push_back(*it);
		} catch(...) {
			mutex.unlock();
			throw;
		}
		mutex.unlock();
		return r;
	}

};

}
	
}

#endif  // __UTILS_VECTOR__

