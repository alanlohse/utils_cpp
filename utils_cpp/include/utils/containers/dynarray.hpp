#ifndef __UTILS_ARRAY__
#define __UTILS_ARRAY__

#include <iterator>
#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>

namespace utils {

namespace containers {
	
template <typename _T, class _Alloc = DEFAULT_ALLOCATOR<_T>, class _Mutex = null_mutex>
class dynarray {
public:
	typedef _T value_type;
	typedef _T& reference;
	typedef const _T& const_reference;
	typedef _T* pointer;
	typedef const _T* const_pointer;
	typedef _T* iterator;
	typedef const _T* const_iterator;
	typedef std::reverse_iterator<iterator>	reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Alloc allocator_type;
private:
	pointer _begin;
	pointer _end;
	allocator_type _allocator;
	_Mutex mutex;
public:
	dynarray(size_type size, const value_type& value = value_type()) : _begin(), _end(), _allocator(), mutex() {
		_begin = _allocator.allocate(size);
		_end = _begin + size;
		pointer it = _begin;
		for (; it < _end; ++it)
			_allocator.construct(it,value);
	}
	template<class _Iterator>
	dynarray(_Iterator begin, _Iterator end) : _begin(), _end(), _allocator(), mutex() {
		size_type size = end-begin;
		_begin = _allocator.allocate(size);
		_end = _begin + size;
		pointer itd = _begin;
		_Iterator its = begin;
		for (; itd < _end; ++itd, ++its)
			_allocator.construct(itd,*its);
	}
	dynarray(const dynarray& other) : _begin(), _end(), _allocator(), mutex() {
		dynarray::const_iterator begin = other.begin();
		dynarray::const_iterator end = other.end();
		size_type size = end-begin;
		_begin = _allocator.allocate(size);
		_end = _begin + size;
		pointer itd = _begin;
		dynarray::const_iterator its = begin;
		for (; itd < _end; ++itd, ++its)
			_allocator.construct(itd,*its);
	}
	~dynarray() {
		pointer it = _begin;
		for (; it < _end; ++it)
			_allocator.destroy(it);
		_allocator.deallocate(_begin,_end-_begin);
	}
	
	iterator begin() {
		return _begin;
	}
	
	iterator end() {
		return _end;
	}
	
	const_iterator begin() const {
		return _begin;
	}
	
	const_iterator end() const {
		return _end;
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
	
	value_type* data() {
		return _begin;
	}
	const value_type* data() const {
		return _begin;
	}
	
	size_type size() const {
		return _end - _begin;
	}
	size_type max_size() const {
		return _end - _begin;
	}
	bool empty() const {
		return _begin == _end;
	}

	reference operator[](size_type n) { 
		return *(begin() + n);
	}

	const_reference operator[](size_type n) const {
		return *(begin() + n);
	}

	reference at(size_type n) {
		return *(begin() + n);
	}
	const_reference at(size_type n) const {
		return *(begin() + n);
	}

	reference front() {
		return *_begin;
	}
	const_reference front() const {
		return *_begin;
	}

	reference back() {
		return *(_end-1);
	}
	const_reference back() const {
		return *(_end-1);
	}

	template<typename _T2, class _Alloc2, class _Mutex2>
	void copy(dynarray<_T2,_Alloc2,_Mutex2>& other) {
		mutex.lock();
		other.mutex.lock();
		const_iterator src = begin();
		const_iterator send = end();
		typename dynarray<_T2,_Alloc2,_Mutex2>::const_iterator dst = other.begin();
		typename dynarray<_T2,_Alloc2,_Mutex2>::const_iterator dend = other.end();
		while (src != send && dst != dend)
			*(dst++) = *(src++);
		other.mutex.unlock();
		mutex.unlock();
	}
		
	void fill (const value_type& val) {
		mutex.lock();
		iterator it = begin();
		iterator end = end();
		for (; it != end; ++it)
			*it = val;
		mutex.unlock();
	}
	
	void swap (dynarray& x) {
		mutex.lock();
		x.mutex.lock();
		std::swap(_begin,x._begin);
		std::swap(_end,x._end);
		std::swap(_allocator,x._allocator);
		x.mutex.unlock();
		mutex.unlock();
	}
	
	dynarray& assign(dynarray& x) {
		swap(x);
		return *this;
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
	
	template<class _Predicate >
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
	size_type count(_Predicate pred) {
		size_type r = 0;
		mutex.try_lock();
		try {
			iterator it = begin();
			iterator finis = end();
			for (; it != finis; ++it)
				if (pred(*it))
					++r;
		} catch(...) {
			mutex.unlock();
			throw;
		}
		mutex.unlock();
		return r;
	}

};
	
} // namespace containers
	
} // namespace utils 

#endif // __UTILS_ARRAY__
