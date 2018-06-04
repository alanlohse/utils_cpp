/*
 * hash_map.hpp
 *
 *  Created on: 26/04/2011
 *      Author: Alan
 */

#ifndef HASH_MAP_HPP_
#define HASH_MAP_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <vector>

namespace utils {

namespace containers {

template<typename _K, typename _V, class _Hash = utils::hash<_K>, class _Equal = std::equal_to<_K>,
	class _Alloc = std::allocator<std::pair<_K,_V> >, class _Mutex = utils::null_mutex>
class hashmap;

template <typename _V>
struct _hashmap_node {
	_V value;
	int _hash;
	_hashmap_node* next;
	_hashmap_node() : value(), _hash(0), next(NULL) { }
	_hashmap_node(_V _value, _hashmap_node* _next) : value(_value), _hash(0), next(_next) { }
	_hashmap_node(const _hashmap_node& other) : value(other.value), _hash(0), next(other.next) { }
	~_hashmap_node() { }
};

template <class _K, class _V, class _HashF, class _EqualK, class _Alloc, class _Mutex>
struct _hash_map_const_iterator;


template <typename _K, typename _V, class _Hash, class _Equal, class _Alloc, class _Mutex>
struct _hash_map_iterator {
	typedef std::pair<_K, _V> value_type;
	typedef std::forward_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef value_type& reference;
	typedef value_type* pointer;

	typedef hashmap<_K, _V, _Hash, _Equal, _Alloc, _Mutex> _hash_map;
	typedef _hash_map_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> iterator;
	typedef _hash_map_const_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> const_iterator;
	typedef _hashmap_node<std::pair<_K, _V> > _node_type;

	_node_type* _node;
	_node_type** _buck;
	_node_type** _end_bucks;

	_hash_map_iterator() : _node(0), _buck(0), _end_bucks(0) { }
	_hash_map_iterator(_node_type* node, _node_type** buck, _node_type** end_bucks) :
		_node(node), _buck(buck), _end_bucks(end_bucks) { }
	_hash_map_iterator(const _hash_map_iterator& other) :
		_node(other._node), _buck(other._buck), _end_bucks(other._end_bucks) { }
	_hash_map_iterator(const const_iterator& other) :
		_node(other._node), _buck(other._buck), _end_bucks(other._end_bucks) { }

	reference operator*() const {
		return _node->value;
	}
	pointer operator->() const {
		return &(operator*());
	}
	iterator& operator++() {
		_node = _node->next;
		if (!_node) {
			while (_buck < _end_bucks - 1) {
				++_buck;
				if (*_buck) {
					_node = *_buck;
					break;
				}
			}
		}
		return *this;
	}
	iterator operator++(int) {
		iterator _tmp = *this;
		++*this;
		return _tmp;
	}
	iterator& operator+=(const difference_type& __n) {
		for (difference_type i = 0; i < __n; i++)
			operator++();
		return *this;
	}
	iterator operator+(const difference_type& __n) const {
		iterator _it(*this);
		_it += __n;
		return _it;
	}
	bool operator==(const iterator& _it) const {
		return _node == _it._node;
	}
	bool operator!=(const iterator& _it) const {
		return _node != _it._node;
	}

};

template <typename _K, typename _V, class _Hash, class _Equal, class _Alloc, class _Mutex>
struct _hash_map_const_iterator {

	typedef std::forward_iterator_tag iterator_category;
	typedef std::pair<_K, _V> value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;

	typedef hashmap<_K, _V, _Hash, _Equal, _Alloc, _Mutex> _hash_map;
	typedef _hash_map_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> iterator;
	typedef _hash_map_const_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> const_iterator;
	typedef _hashmap_node<std::pair<_K, _V> > _node_type;

	_node_type* _node;
	_node_type** _buck;
	_node_type** _end_bucks;

	_hash_map_const_iterator() : _node(0), _buck(0), _end_bucks(0) { }
	_hash_map_const_iterator(_node_type* node, _node_type** buck, _node_type** end_bucks) :
		_node(node), _buck(buck), _end_bucks(end_bucks) { }
	_hash_map_const_iterator(const _hash_map_const_iterator& other) :
		_node(other._node), _buck(other._buck), _end_bucks(other._end_bucks) { }
	_hash_map_const_iterator(const iterator& other) :
		_node(other._node), _buck(other._buck), _end_bucks(other._end_bucks) { }

	reference operator*() const {
		return _node->value;
	}
	pointer operator->() const {
		return &(operator*());
	}
	const_iterator& operator++() {
		_node = _node->next;
		if (!_node) {
			while (_buck < _end_bucks - 1) {
				++_buck;
				if (*_buck) {
					_node = *_buck;
					break;
				}
			}
		}
		return *this;
	}
	const_iterator operator++(int) {
		const_iterator _tmp = *this;
		++*this;
		return _tmp;
	}
	const_iterator& operator+=(const difference_type& __n) {
		for (difference_type i = 0; i < __n; i++)
			operator++();
		return *this;
	}
	const_iterator operator+(const difference_type& __n) const {
		const_iterator _it(*this);
		_it += __n;
		return _it;
	}
	bool operator==(const iterator& _it) const {
		return _node == _it._node;
	}
	bool operator!=(const iterator& _it) const {
		return _node != _it._node;
	}

};

template<typename _K, typename _V, class _Hash, class _Equal, class _Alloc, class _Mutex>
class hashmap {
public:
	typedef _K key_type;
	typedef _V mapped_type;
	typedef std::pair<_K,_V> value_type;
	typedef _Hash hasher;
	typedef _Equal key_equal;
	typedef _hash_map_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> iterator;
	typedef _hash_map_const_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex> const_iterator;
	typedef typename _Alloc:: template rebind<value_type>::other allocator_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef _Mutex mutex_type;

private:
	friend struct _hash_map_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex>;
	friend struct _hash_map_const_iterator<_K, _V, _Hash, _Equal, _Alloc, _Mutex>;
	typedef _hashmap_node<std::pair<_K, _V> > _node_type;
	typedef typename _Alloc::template rebind<_node_type>::other node_allocator_type;
	typedef typename _Alloc::template rebind<_node_type*>::other bucket_allocator_type;

	node_allocator_type node_allocator;
	bucket_allocator_type bucket_allocator;
	_node_type** _buckets;
	size_type _buck_count;
	size_type _count;
	float _max_load_factor;
	hasher _hasher;
	key_equal _key_equal;
	mutex_type mutex;

	_node_type* _find_previos(_node_type* node,_node_type** buck) {
		_node_type* cur = *buck;
		if (cur == node)
			return NULL;
		while (cur && cur->next != node) cur = cur->next;
		return cur;
	}

	void resize(size_type n) {
		if (n / _max_load_factor < _buck_count) {
			n = utils::next_prime(n);
			rehash(n);
		}
	}

	void _insert_node(_node_type** n_buckets, size_type n, _node_type* cur) {
		size_type pos = cur->_hash % n;
		cur->next = n_buckets[pos];
		n_buckets[pos] = cur;
	}

	value_type& _find_or_insert(const key_type& k) {
		mutex.lock();
		value_type obj(k, mapped_type());
		int _h = _hasher(obj.first);
		int pos = _h % _buck_count;
		_node_type* _node = _buckets[pos];
		for (; _node; _node = _node->next) {
			if (_key_equal(obj.first, _node->value.first)) {
				mutex.unlock();
				return _node->value;
			}
		}
		resize(++_count);
		pos = _h % _buck_count;
		_node = node_allocator.allocate(1);
		node_allocator.construct(_node,_node_type(obj,_buckets[pos]));
		_node->_hash = _h;
		_buckets[pos] = _node;
		mutex.unlock();
		return _node->value;
	}

	template<typename _K2, typename _V2, class _Hash2, class _Equal2, class _Alloc2, class _Mutex2>
	void _assign(hashmap<_K2, _V2, _Hash2, _Equal2, _Alloc2, _Mutex2>& o) {
		mutex.lock();
		o.mutex.lock();
		clear();
		typename hashmap<_K2, _V2, _Hash2, _Equal2, _Alloc2, _Mutex2>::iterator it = o.begin();
		typename hashmap<_K2, _V2, _Hash2, _Equal2, _Alloc2, _Mutex2>::iterator end = o.end();
		for (; it != end; ++it)
			insert(*it);
		mutex.unlock();
		o.mutex.unlock();
		return *this;
	}


public:
	hashmap(size_type n = 37, const hasher& hsh = hasher(), const key_equal& ke = key_equal()) :
		node_allocator(),
		bucket_allocator(),
		_buckets(0),
		_buck_count(n),
		_count(0),
		_max_load_factor(0.75f),
		_hasher(hsh),
		_key_equal(ke), mutex() {
		_buckets = bucket_allocator.allocate(n);
		::memset(_buckets,0,sizeof(_node_type*)*n);
	}

	template<typename _K2, typename _V2, class _Hash2, class _Equal2, class _Alloc2, class _Mutex2>
	hashmap(hashmap<_K2, _V2, _Hash2, _Equal2, _Alloc2, _Mutex2>& o) :
		node_allocator(),
		bucket_allocator(),
		_buckets(0),
		_buck_count(o.size()),
		_count(0),
		_max_load_factor(0.75f),
		_hasher(o._hasher),
		_key_equal(o._key_equal), mutex() {
		_buckets = bucket_allocator.allocate(o.size());
		::memset(_buckets,0,sizeof(_node_type*)*o.size());
		_assign(o);
	}

	~hashmap() {
		mutex.lock();
		clear();
		bucket_allocator.deallocate(_buckets,_buck_count);
		mutex.unlock();
	}

	void clear() {
		mutex.lock();
		_node_type** buck = _buckets;
		_node_type** endbuck = _buckets + _buck_count;
		do {
			_node_type* cur = *buck;
			while (cur) {
				_node_type* next = cur->next;
				node_allocator.destroy(cur);
				node_allocator.deallocate(cur,1);
				cur = next;
			}
			*buck = NULL;
			++buck;
		} while (buck < endbuck);
		mutex.unlock();
	}

	iterator begin() {
		_node_type** buck = _buckets;
		_node_type** endbuck = _buckets + _buck_count;
		for (; buck < endbuck; ++buck)
			if (*buck)
				return iterator(*buck, buck, endbuck);
		return iterator(NULL, endbuck, endbuck);
	}

	iterator end() {
		_node_type** endbuck = _buckets + _buck_count;
		return iterator(NULL, endbuck, endbuck);
	}

	const_iterator begin() const {
		_node_type** buck = _buckets;
		_node_type** endbuck = _buckets + _buck_count;
		for (; buck < endbuck; ++buck)
			if (*buck)
				return const_iterator(*buck, buck, endbuck);
		return const_iterator(NULL, endbuck, endbuck);
	}

	const_iterator end() const {
		_node_type** endbuck = _buckets + _buck_count;
		return const_iterator(NULL, endbuck, endbuck);
	}

	iterator find(const key_type& k) {
		mutex.try_lock();
		int _h = _hasher(k);
		int pos = _h % _buck_count;
		_node_type* _node = _buckets[pos];
		for (; _node; _node = _node->next) {
			if (_key_equal(k, _node->value.first)) {
				mutex.unlock();
				return iterator(_node, _buckets + pos, _buckets + _buck_count);
			}
		}
		mutex.unlock();
		return end();
	}
	const_iterator find(const key_type& k) const {
		mutex.try_lock();
		int _h = _hasher(k);
		int pos = _h % _buck_count;
		_node_type* _node = _buckets[pos];
		for (; _node; _node = _node->next) {
			if (_key_equal(k, _node->value.first)) {
				mutex.unlock();
				return const_iterator(_node, _buckets + pos, _buckets + _buck_count);
			}
		}
		mutex.unlock();
		return end();
	}

	std::pair<iterator,bool> insert(const value_type& val){
		mutex.lock();
		int _h = _hasher(val.first);
		int pos = _h % _buck_count;
		_node_type* _node = _buckets[pos];
		for (; _node; _node = _node->next) {
			if (_key_equal(val.first, _node->value.first)) {
				_node->value = val;
				mutex.unlock();
				return std::pair<iterator,bool>(iterator(_node, _buckets + pos, _buckets + _buck_count),false);
			}
		}
		resize(++_count);
		pos = _h % _buck_count;
		_node = node_allocator.allocate(1);
		node_allocator.construct(_node,_node_type(val,_buckets[pos]));
		_node->_hash = _h;
		_buckets[pos] = _node;
		mutex.unlock();
		return std::pair<iterator,bool>(iterator(_node, _buckets + pos, _buckets + _buck_count),true);
	}

	size_type erase(const key_type& key) {
		mutex.lock();
		iterator it = find(key);
		if (it != end()) {
			erase(it);
			mutex.unlock();
			return 1;
		}
		mutex.unlock();
		return 0;
	}

	iterator erase(const_iterator pos) {
		mutex.lock();
		_node_type* node = pos._node;
		_node_type** buck = pos._buck;
		_node_type* prev = _find_previos(node,buck);
		_node_type* next = node->next;
		if (prev)
			prev->next = next;
		else // if (next)
			*buck = next;
//		else {
//			do {
//				buck++;
//			} while(*buck == NULL && buck < pos._end_bucks);
//			if (buck < pos._end_bucks)
//				next = *buck;
//		}
		node_allocator.destroy(node);
		node_allocator.deallocate(node,1);
		mutex.unlock();
		return iterator(next,buck,pos._end_bucks);
	}

	mapped_type& operator[](const key_type& k) {
		return _find_or_insert(k).second;
	}

	// bucket interface
	size_type bucket_count() const {
		return _buck_count;
	}
	size_type max_bucket_count() const {
		return size_type(-1);
	}
	size_type bucket_size(size_type n) {
		_node_type* _node = _buckets[n];
		size_type _c = 0;
		for (; _node; _node = _node->next)
			++_c;
		return _c;
	}
	size_type bucket(const key_type& k) {
		return _hasher(k) % _buck_count;
	}

	// hash policy
	float load_factor() const {
		return 1.0f * _count / _buck_count;
	}
	float max_load_factor() const {
		return _max_load_factor;
	}
	void max_load_factor(float z) {
		_max_load_factor = z;
		resize(_count);
	}
	size_type size() const {
		return _count;
	}
	void rehash(size_type n) {
		mutex.lock();
		if (n > _count / _max_load_factor && n != _buck_count) {
			_node_type** n_buckets = bucket_allocator.allocate(n);
			::memset(n_buckets,0,sizeof(_node_type*)*n);
			_node_type** buck = _buckets;
			_node_type** endbuck = _buckets + _buck_count;
			do {
				_node_type* cur = *buck;
				while (cur) {
					_node_type* next = cur->next;
					_insert_node(n_buckets,n,cur);
					cur = next;
				}
				*buck = NULL;
				++buck;
			} while (buck < endbuck);
			bucket_allocator.deallocate(_buckets,_buck_count);
			_buckets = n_buckets;
			_buck_count = n;
		}
		mutex.unlock();
	}

	void swap(hashmap& o) {
		mutex.lock();
		o.mutex.lock();
		utils::swap(node_allocator,o.node_allocator);
		utils::swap(bucket_allocator,o.bucket_allocator);
		utils::swap(_buckets,o._buckets);
		utils::swap(_buck_count,o._buck_count);
		utils::swap(_count,o._count);
		utils::swap(_max_load_factor,o._max_load_factor);
		utils::swap(_hasher,o._hasher);
		utils::swap(_key_equal,o._key_equal);
		mutex.unlock();
		o.mutex.unlock();
	}

	template<typename _K2, typename _V2, class _Hash2, class _Equal2, class _Alloc2, class _Mutex2>
	hashmap& operator = (hashmap<_K2, _V2, _Hash2, _Equal2, _Alloc2, _Mutex2>& o) {
		_assign(o);
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
	hashmap filter(_Predicate pred) {
		hashmap r;
		mutex.try_lock();
		try {
			iterator it = begin();
			iterator finis = end();
			for (; it != finis; ++it)
				if (pred(*it))
					r.insert(*it);
		} catch(...) {
			mutex.unlock();
			throw;
		}
		mutex.unlock();
		return r;
	}

};


template <class Key, class T, class Hash, class Pred, class Alloc, class _Mutex>
inline void swap(hashmap<Key, T, Hash, Pred, Alloc, _Mutex>& x,
		hashmap<Key, T, Hash, Pred, Alloc, _Mutex>& y) {
	x.swap(y);
}

}

}

#endif /* HASH_MAP_HPP_ */
