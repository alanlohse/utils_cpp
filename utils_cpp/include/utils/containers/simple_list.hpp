/*
 * simple_list.hpp
 *
 *  Created on: 23/04/2015
 *      Author: Alan
 */

#ifndef SIMPLE_LIST_HPP_
#define SIMPLE_LIST_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/exception.hpp>
#include <new>
#include <iterator>

namespace utils {

namespace containers {

template<typename _T>
struct simple_fwdlist_node {
	_T value;
	simple_fwdlist_node* next;
	simple_fwdlist_node(const _T& v): value(v), next(NULL) { }
	simple_fwdlist_node(const _T& v, simple_fwdlist_node* n): value(v), next(n) { }
};

template<typename _T>
struct fwdlist_iterator {
	typedef fwdlist_iterator 									iterator;
    typedef typename
		std::iterator_traits<_T*>::iterator_category		iterator_category;
    typedef typename
		std::iterator_traits<_T*>::value_type  			value_type;
    typedef typename
		std::iterator_traits<_T*>::difference_type			difference_type;
    typedef typename std::iterator_traits<_T*>::reference	reference;
    typedef typename std::iterator_traits<_T*>::pointer	pointer;

    simple_fwdlist_node<_T>* node;
	fwdlist_iterator(): node(NULL) { }
	fwdlist_iterator(simple_fwdlist_node<_T>* _n): node(_n) { }
	reference operator * () const {
		return node->value;
	}
	pointer operator -> () const {
		return &node->value;
	}
	iterator& operator ++ () {
		if (!node)
			throw boundary_exceeded_exception();
		if (node) node = node->next;
		return *this;
	}
	iterator& operator ++ (int) {
		iterator tmp = *this;
		++(*this);
		return tmp;
	}
	iterator& operator+=(difference_type n) {
		for (; n >= 0; --n) {
			if (!node)
				throw boundary_exceeded_exception();
			node = node->next;
		}
		return *this;
	}
	iterator operator+(difference_type n) {
		iterator __tmp = *this;
		__tmp += n;
		return __tmp;
	}
	bool operator==(const iterator& __it) const {
		return node == __it.node;
	}
	bool operator!=(const iterator& __it) const {
		return node != __it.node;
	}
	difference_type operator-(const iterator& __it) const {
		simple_fwdlist_node<_T>* cur = node;
		for (difference_type n = 0; cur; n++, cur = node->next)
			if (cur == __it.node)
				return n;
		return -1;
	}
	bool operator ! () const {
		return node == NULL;
	}
	operator bool() const {
		return node != NULL;
	}

};

template<typename _T>
struct const_fwdlist_iterator {
	typedef const_fwdlist_iterator 									iterator;
    typedef typename
		std::iterator_traits<_T*>::iterator_category		iterator_category;
    typedef typename
		std::iterator_traits<_T*>::value_type  			value_type;
    typedef typename
		std::iterator_traits<_T*>::difference_type			difference_type;
    typedef const typename std::iterator_traits<_T*>::reference	reference;
    typedef const typename std::iterator_traits<_T*>::pointer	pointer;

    simple_fwdlist_node<_T>* node;
	const_fwdlist_iterator(): node(NULL) { }
	const_fwdlist_iterator(simple_fwdlist_node<_T>* _n): node(_n) { }
	reference operator * () const {
		return node->value;
	}
	pointer operator -> () const {
		return &node->value;
	}
	iterator& operator ++ () {
		if (!node)
			throw boundary_exceeded_exception();
		if (node) node = node->next;
		return *this;
	}
	iterator operator ++ (int) {
		iterator tmp = *this;
		++(*this);
		return tmp;
	}
	iterator& operator+=(difference_type n) {
		for (; n >= 0; --n) {
			if (!node)
				throw boundary_exceeded_exception();
			node = node->next;
		}
		return *this;
	}
	iterator operator+(difference_type n) {
		iterator __tmp = *this;
		__tmp += n;
		return __tmp;
	}
	bool operator==(const iterator& __it) const {
		return node == __it.node;
	}
	bool operator!=(const iterator& __it) const {
		return node != __it.node;
	}
	difference_type operator-(const iterator& __it) const {
		simple_fwdlist_node<_T>* cur = node;
		for (difference_type n = 0; cur; n++, cur = node->next)
			if (cur == __it.node)
				return n;
		return -1;
	}
	bool operator ! () const {
		return node == NULL;
	}
	operator void*() const {
		return node;
	}

};

template<typename _Tp, class _Alloc = DEFAULT_ALLOCATOR<_Tp> >
class fwdlist {
public:
	typedef size_t size_type;
	typedef simple_fwdlist_node<_Tp> node_type;
	typedef _Tp value_type;
	typedef _Alloc allocator_type;
	typedef typename allocator_type::template rebind<node_type>::other allocator_node_type;
	typedef fwdlist_iterator<_Tp> iterator;
	typedef const_fwdlist_iterator<_Tp> const_iterator;
    typedef typename allocator_type::pointer                   pointer;
    typedef typename allocator_type::const_pointer             const_pointer;
    typedef typename allocator_type::reference                 reference;
    typedef typename allocator_type::const_reference           const_reference;
    typedef ptrdiff_t					 difference_type;
private:
    node_type* _first_node;
    node_type* _last_node;
    allocator_type _allocator;
    allocator_node_type _node_allocator;
    void remove_node(node_type* prev, node_type* node) {
    	prev->next = node->next;
		_node_allocator.deallocate(node,1);
		if (prev->next == NULL)
			_last_node = prev;
    }

public:
    fwdlist() : _first_node(NULL), _last_node(NULL), _allocator(), _node_allocator() { }
    fwdlist(const fwdlist& other) : _first_node(NULL), _last_node(NULL), _allocator(), _node_allocator() {
    	assign(other.begin(),other.end());
    }
    template <class InputIterator>
    fwdlist(InputIterator first, InputIterator last) : _first_node(NULL), _last_node(NULL), _allocator(), _node_allocator() {
    	assign(first,last);
    }
    ~fwdlist() {
    	clear();
    }

    template <class InputIterator>
    void assign (InputIterator first, InputIterator last) {
    	for (; first != last; first++)
    		push_back(*first);
    }

    void push_front(const _Tp& v) {
		node_type* next = _first_node;
		_first_node = _node_allocator.allocate(1,NULL);
		_node_allocator.construct(_first_node,node_type(v,next));
		if (_last_node == NULL)
			_last_node = _first_node;
    }

    void push_back(const _Tp& v) {
		node_type* prev = _last_node;
		_last_node = _node_allocator.allocate(1,NULL);
		_node_allocator.construct(_last_node,node_type(v));
		if (prev)
			prev->next = _last_node;
		if (_first_node == NULL)
			_first_node = _last_node;
    }

    void pop_front() {
    	if (_first_node) {
    		node_type* node = _first_node->next;
    		_node_allocator.deallocate(_first_node,1);
    		_first_node = node;
    		if (_first_node == NULL)
    			_last_node = NULL;
    	}
    }

    void pop_back() {
    	if (_last_node == _first_node) {
    		pop_front();
    	} else if (_last_node) {
    		node_type* prev = _first_node;
    		while (prev->next != _last_node) prev = prev->next;
    		_node_allocator.deallocate(_last_node,1);
    		_last_node = prev;
    		_last_node->next = NULL;
    	}
    }

    void clear() {
    	node_type* node = _first_node;
    	while (node) {
    		node_type* next = node->next;
    		_node_allocator.deallocate(node,1);
    		node = next;
    	}
    	_first_node = _last_node = NULL;
    }

    bool empty() const {
    	return _first_node == NULL;
    }

    iterator begin() {
    	return iterator(_first_node);
    }

    iterator end() {
    	return iterator(NULL);
    }

    const_iterator begin() const {
    	return const_iterator(_first_node);
    }

    const_iterator end() const {
    	return const_iterator(NULL);
    }

    const_iterator cbegin() const {
    	return const_iterator(_first_node);
    }

    const_iterator cend() const {
    	return const_iterator(NULL);
    }

    reference front() {
    	return _first_node->value;
    }

    const_reference front() const {
    	return _first_node->value;
    }

    reference back() {
    	return _last_node->value;
    }

    const_reference back() const {
    	return _last_node->value;
    }

    void remove(const _Tp& val) {
    	if (_first_node) {
			if (_first_node->value == val) {
				pop_front();
				return;
			}
			if (_last_node->value == val) {
				pop_back();
				return;
			}
			node_type *prev = _first_node;
			node_type *node = _first_node->next;
			for (; node; node = node->next) {
				if (node->value == val) {
					remove_node(prev,node);
					return;
				}
				prev = node;
			}
    	}
    }

    iterator erase_after (const_iterator position) {
    	if (position.node) {
        	node_type *node = _first_node;
        	for (; node && node != position.node; node = node->next);
        	if (node && node->next) {
        		node_type* nextnext = node->next->next;
        		remove_node(node, node->next);
        		return iterator(nextnext);
        	}
    	}
    	return iterator(NULL);
    }

    iterator erase_after (const_iterator position, const_iterator last) {
    	if (position.node) {
        	node_type *node = _first_node;
        	for (; node && node != position.node; node = node->next);
        	if (node && node->next) {
        		node_type* nextnext = node->next->next;
        		for (; node->next != last; node = node->next) {
        			nextnext = node->next->next;
        			remove_node(node, node->next);
        		}
        		return iterator(nextnext);
        	}
    	}
    	return iterator(NULL);
    }
};


} // containers

} // utils

#endif /* SIMPLE_LIST_HPP_ */
