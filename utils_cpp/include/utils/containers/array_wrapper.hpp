#ifndef __UTILS_ARRAY__
#define __UTILS_ARRAY__

#include <iterator>
#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>

namespace utils {

namespace containers {
	
template <typename _T, class _Alloc = DEFAULT_ALLOCATOR<_T>, class _Mutex = null_mutex>
class array_wrapper {
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
	typedef typename _Alloc:: template rebind<value_type>::other allocator_type;
private:
	struct wrapper_data {
		pointer _begin;
		pointer _end;
		allocator_type _allocator;
		difference_type refs;
		wrapper_data(): _begin(), _end(), _allocator(), refs(0) { }
		wrapper_data(wrapper_data& other): _begin(other._begin),
				_end(other._end),
				_allocator(other._allocator), 
				refs(other.refs) { }
		~wrapper_data(){
			if (_begin) {
				pointer it = _begin;
				for (; it < _end; ++it)
					_allocator.destroy(it);
				_allocator.deallocate(_begin,_end-_begin);
			}
		}
		wrapper_data* add_ref() {
			refs++;
			return this;
		}
		wrapper_data* rem_ref() {
			refs--;
			return this;
		}
		bool is_leak() const {
			return refs <= 0;
		}
	};
	typedef typename _Alloc:: template rebind<wrapper_data>::other allocator_data_type;
	wrapper_data* _data;
	allocator_data_type _data_allocator;
	_Mutex mutex;
public:
	array_wrapper() : _data(NULL), _data_allocator(), mutex() {
	}
	template<class _Iterator>
	array_wrapper(_Iterator begin, _Iterator end) : _data(NULL), _data_allocator(), mutex() {
		_data = _data_allocator.allocate(1);
		::new (_data) wrapper_data();
		_data->_begin = &(*begin);
		_data->_end = &(*end);
		_data->refs++;
	}
	array_wrapper(const array_wrapper& other) : _data(other._data->add_ref()), _data_allocator(), mutex() {
	}
	~array_wrapper() {
		if (_data && _data->rem_ref()->is_leak()) {
			_data->~wrapper_data();
			_data_allocator.deallocate(_data, 1);
		}
	}
	
	iterator begin() {
		return _data ? _data->_begin : NULL;
	}
	
	iterator end() {
		return _data ? _data->_end : NULL;
	}
	
	const_iterator begin() const {
		return _data ? _data->_begin : NULL;
	}
	
	const_iterator end() const {
		return _data ? _data->_end : NULL;
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
		return _data->_begin;
	}
	const value_type* data() const {
		return _data->_begin;
	}
	
	size_type size() const {
		return _data->_end - _data->_begin;
	}
	size_type max_size() const {
		return _data->_end - _data->_begin;
	}
	bool empty() const {
		return _data->_begin == _data->_end;
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
		return *_data->_begin;
	}
	const_reference front() const {
		return *_data->_begin;
	}

	reference back() {
		return *(_data->_end-1);
	}
	const_reference back() const {
		return *(_data->_end-1);
	}

	template<typename _T2, class _Alloc2, class _Mutex2>
	void copy(array_wrapper<_T2,_Alloc2,_Mutex2>& other) {
		mutex.lock();
		other.mutex.lock();
		const_iterator src = begin();
		const_iterator send = end();
		typename array_wrapper<_T2,_Alloc2,_Mutex2>::const_iterator dst = other.begin();
		typename array_wrapper<_T2,_Alloc2,_Mutex2>::const_iterator dend = other.end();
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
	
	void swap (array_wrapper& x) {
		mutex.lock();
		x.mutex.lock();
		std::swap(_data,x._data);
		std::swap(_data_allocator,x._data_allocator);
		x.mutex.unlock();
		mutex.unlock();
	}
	
	array_wrapper& assign(const array_wrapper& x) {
		if (_data && _data->rem_ref()->is_leak()) {
			_data->~wrapper_data();
			_data_allocator.deallocate(_data, 1);
		}
		_data = (x._data ? x._data->add_ref() : NULL);
		return *this;
	}

	array_wrapper& operator = (const array_wrapper& x) {
		return assign(x);
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
