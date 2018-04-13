/*
 * queue.h
 *
 *  Created on: 20/06/2015
 *      Author: ALAN
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/exception.hpp>
#include <utility>
#include <memory>

namespace utils {

namespace containers {


template<typename _T, class _Alloc = std::allocator<_T>,
		class _Mutex = utils::null_mutex>
class queue {
public:
	typedef _T value_type;
	typedef size_t size_type;
	typedef _Mutex mutex_type;
protected:
	struct _queue_node {
		value_type value;
		_queue_node* next;
		_queue_node() : value(), next(0) { }
	};

	typedef typename _Alloc::template rebind<_queue_node<_T> >::other node_allocator_type;

	_queue_node* _first;
	_queue_node* _last;
	node_allocator_type _allocator;
	mutex_type mutex;
	size_type _count;

public:
	queue() :
			_first(), _last(), _allocator(), mutex(), _count(0) {
	}

	~queue() {
		while (!empty())
			pop();
	}

	bool empty() const {
		return _first == NULL;
	}

	size_type size() const {
		return _count;
	}

	value_type& front() {
		if (_first)
			return _first->value;
		throw utils::boundary_exceeded_exception("queue is empty");
	}
	const value_type& front() const {
		if (_first)
			return _first->value;
		throw utils::boundary_exceeded_exception("queue is empty");
	}

	value_type& back() {
		if (_last)
			return _last->value;
		throw utils::boundary_exceeded_exception("queue is empty");
	}
	const value_type& back() const {
		if (_last)
			return _last->value;
		throw utils::boundary_exceeded_exception("queue is empty");
	}
	void push(const value_type& val) {
		mutex.lock();
		_queue_node* node = _allocator.allocate(1);
		_allocator.construct(node,_queue_node());
		node->value = val;
		_last->next = node;
		_last = node;
		++_count;
		mutex.unlock();
	}
	void pop() {
		if (_first) {
			mutex.lock();
			_queue_node* next = _first->next;
			if (_first == _last)
				_last = NULL;
			_allocator.destroy(_first);
			_allocator.deallocate(_first,1);
			_first = next;
			--_count;
			mutex.unlock();
		} else
			throw utils::boundary_exceeded_exception("queue is empty");
	}
	void swap(queue& other) {
		mutex.lock();
		utils::swap(_first,other._first);
		utils::swap(_last,other._last);
		utils::swap(_allocator,other._allocator);
		utils::swap(_count,other._count);
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

};


}  // namespace containers

}  // namespace utils


#endif /* QUEUE_H_ */
