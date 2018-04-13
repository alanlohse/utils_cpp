#ifndef __UTILS_ARRAY__
#define __UTILS_ARRAY__

#include <iterator>
#include <utils/funcs.hpp>

namespace utils {

namespace containers {
	
template <typename _T, size_t _Sz = 1, class _Mutex = null_mutex>
class array {
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
private:
	_T _value[_Sz];
	_Mutex mutex;
public:
	array() : _value(),  mutex() {
	}
	array(const value_type* value, size_type length = _Sz) : _value(),  mutex() {
		if (length > _Sz) length = _Sz;
		memcpy(_value,value,sizeof(value_type)*length);
	}
	array(const array& other) : _value(),  mutex() {
		memcpy(_value,other._value,sizeof(value_type)*_Sz);
	}
	~array() { }
	
	iterator begin() {
		return _value;
	}
	
	iterator end() {
		return _value + _Sz;
	}
	
	const_iterator begin() const {
		return _value;
	}
	
	const_iterator end() const {
		return _value + _Sz;
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
		return _value;
	}
	const value_type* data() const {
		return _value;
	}
	
	size_type size() const {
		return _Sz;
	}
	size_type max_size() const {
		return _Sz;
	}
	bool empty() const {
		return _Sz == 0;
	}

	reference operator[](size_type n) { 
		return const_cast<_T&>(_value[n]); 
	}

	const_reference operator[](size_type n) const {
		return const_cast<const _T&>(_value[n]);
	}

	reference at(size_type n) {
		return const_cast<_T&>(_value[n]); 
	}
	const_reference at(size_type n) const {
		return const_cast<const _T&>(_value[n]);
	}

	reference front() {
		return const_cast<_T&>(_value[0]);
	}
	const_reference front() const {
		return const_cast<const _T&>(_value[0]);
	}

	reference back() {
		return const_cast<_T&>(_value[_Sz-1]);
	}
	const_reference back() const {
		return const_cast<const _T&>(_value[_Sz-1]);
	}

	template<typename _T2, size_t _Sz2, class _Mutex2>
	void copy(array<_T2,_Sz2,_Mutex2>& other) {
		mutex.lock();
		other.mutex.lock();
		const_iterator src = begin();
		const_iterator send = end();
		typename array<_T2,_Sz2,_Mutex2>::iterator dst = other.begin();
		typename array<_T2,_Sz2,_Mutex2>::iterator dend = other.end();
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
	
	void swap (array& x) {
		mutex.lock();
		x.mutex.lock();
		for (size_type i = 0; i < _Sz; ++i)
			std::swap(_value[i],x._value[i]);
		x.mutex.unlock();
		mutex.unlock();
	}
	
	void move(array& x) {
		mutex.lock();
		x.mutex.lock();
		for (size_type i = 0; i < _Sz; ++i)
			x._value[i] = _value[i];
		x.mutex.unlock();
		mutex.unlock();
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
