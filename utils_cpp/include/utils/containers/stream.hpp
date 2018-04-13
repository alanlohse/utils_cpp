/*
 * stream.hpp
 *
 *  Created on: 20/01/2016
 *      Author: Alan
 */

#ifndef INCLUDE_UTILS_CONTAINERS_STREAM_HPP_
#define INCLUDE_UTILS_CONTAINERS_STREAM_HPP_

#include <iterator>
#include <utils/funcs.hpp>
#include <utils/thread/thread.hpp>
#include <vector>
#include <set>

namespace utils {

namespace containers {

struct single_thread_parallel_strategy {

	int max_thread_count() const {
		return 1;
	}

	template <typename _FunctionT>
	void start_thread(_FunctionT* function) {
		(*function)();
	}

	void join(int timeout = -1) {
	}

};

struct free_thread_parallel_strategy {

	int _max_thread_count;
	thread_group _group;
	thread** threads;

	free_thread_parallel_strategy(int ___max_thread_count = 4) : _max_thread_count(___max_thread_count), _group(), threads(new thread*[_max_thread_count]) {
		for (int i = 0; i < _max_thread_count; i++) {
			threads[i] = NULL;
		}
	}

	free_thread_parallel_strategy(const free_thread_parallel_strategy& other) : _max_thread_count(other._max_thread_count), _group(), threads(new thread*[_max_thread_count]) {
		for (int i = 0; i < _max_thread_count; i++) {
			threads[i] = NULL;
		}
	}

	~free_thread_parallel_strategy() {
		_group.~thread_group();
		if (threads) {
			for (int i = 0; i < _max_thread_count; i++)
				if (threads[i])
					delete threads[i];
			delete threads;
		}
	}

	int max_thread_count() const {
		return _max_thread_count;
	}

	template <typename _FunctionT>
	void start_thread(_FunctionT* function) {
		for (int i = 0; i < _max_thread_count; i++)
			if (threads[i] == NULL) {
				threads[i] = new thread(_group,function);
				threads[i]->start();
				break;
			}
	}

	void join(int timeout = -1) {
		_group.wait_all(timeout);
	}

};

template<typename _Tp>
void insert_at_end(const _Tp& value, std::vector<_Tp>& collection) {
	collection.push_back(value);
}

template<typename _Tp>
void insert_at_end(const _Tp& value, std::set<_Tp>& collection) {
	collection.insert(value);
}

template<typename _Tp>
void insert_at_end(const _Tp& value, std::multiset<_Tp>& collection) {
	collection.insert(value);
}

template <typename _CollectionT, typename _ParallelStrategy = single_thread_parallel_strategy>
class stream {
public:
	typedef _CollectionT collection_type;
	typedef typename _CollectionT::value_type value_type;
	typedef typename _CollectionT::iterator iterator;
	typedef typename _CollectionT::const_iterator const_iterator;
	typedef _ParallelStrategy parallel_strategy_type;

	static int minimum_partition_size() {
		return 10;
	}

private:
	collection_type _collection;
	parallel_strategy_type _parallel;

	template <typename _Function>
	struct _partition_worker {
		iterator it, end;
		_Function function;
		collection_type* collection;
		collection_type* dest_collection;
		_partition_worker(iterator _it, iterator _end, _Function _function, collection_type* __collection, collection_type* _dest_collection) : it(_it), end(_end), function(_function), collection(__collection), dest_collection(_dest_collection) {
		}
		_partition_worker(const _partition_worker& other) : it(other.it), end(other.end), function(other.function), collection(other.collection), dest_collection(other.dest_collection) {
		}
		_partition_worker& operator = (const _partition_worker& other){
			it = other.it;
			end = other.end;
			function = other.function;
			collection = other.collection;
			dest_collection = other.dest_collection;
			return *this;
		}
		void operator()() {
			function(it,end,*collection,*dest_collection);
		}
	};

	template <typename _Function>
	void do_it(_Function& function,collection_type& dest_collection) {
		function(_collection.begin(), _collection.end(), _collection, dest_collection);
	}

	template <typename _Function>
	void do_parallel(_Function& function,collection_type& dest_collection) {
		std::vector<collection_type> cols;
		int maxt = _parallel.max_thread_count();
		int tc = _collection.size() / maxt;
		while (tc < minimum_partition_size() && maxt > 1) {
			--maxt;
			tc = _collection.size() / maxt;
		}
		int lastp = _collection.size() % maxt;
		std::vector<_partition_worker<_Function> > _workers;
		for (int i = 0; i < maxt; ++i) {
			cols.push_back(collection_type());
			_partition_worker<_Function> po(_collection.begin() + i * tc, _collection.begin() + (i + 1) * tc + lastp,function,&_collection,&cols[i]);
			_workers.push_back(po);
			lastp = 0;
			_parallel.start_thread(&_workers[i]);
		}
		_parallel.join();
		for (size_t i = 0; i < cols.size(); i++) {
			dest_collection.insert(cols[i].begin(), cols[i].begin(), cols[i].end());
		}
	}

	template <typename _FunctionT>
	struct _filter_function {
		_FunctionT* function;
		_filter_function(_FunctionT* _function) : function(_function) {
		}
		void operator()(iterator it, iterator end, collection_type& collection, collection_type& dest_collection) {
			for (; it != end; ++it)
				if ((*function)(*it))
					insert_at_end<value_type>(*it,dest_collection);
		}
	};

	template <typename _FunctionT>
	struct _foreach_function {
		_FunctionT* function;
		_foreach_function(_FunctionT* _function) : function(_function) {
		}
		void operator()(iterator it, iterator end, collection_type& collection, collection_type& dest_collection) {
			for (; it != end; ++it)
				(*function)(*it);
		}
	};
public:
	stream(const collection_type& collection, const parallel_strategy_type& parallel = parallel_strategy_type()) :
		_collection(collection),
		_parallel(parallel) {
	}

	stream(const stream& other) :
		_collection(other._collection),
		_parallel(other._parallel) {
	}

	~stream() { }

	template <typename _FunctionT>
	stream& filter(_FunctionT function) {
		_filter_function<_FunctionT> filter_part(&function);
		collection_type dest_collection;
		if (_parallel.max_thread_count() == 1)
			do_it(filter_part,dest_collection);
		else
			do_parallel(filter_part,dest_collection);
		_collection = dest_collection;
		return *this;
	}

	template <typename _FunctionT>
	stream& filter(_FunctionT* function) {
		_filter_function<_FunctionT> filter_part(function);
		collection_type dest_collection(_collection.size());
		if (_parallel.max_thread_count() == 1)
			do_it(filter_part,dest_collection);
		else
			do_parallel(filter_part,dest_collection);
		_collection = dest_collection;
		return *this;
	}

	template <typename _FunctionT>
	stream& forEach(_FunctionT function) {
		_foreach_function<_FunctionT> foreach_part(&function);
		collection_type dest_collection(_collection.size());
		if (_parallel.max_thread_count() == 1)
			do_it(foreach_part,dest_collection);
		else
			do_parallel(foreach_part,dest_collection);
		_collection = dest_collection;
		return *this;
	}

	template <typename _FunctionT>
	stream& forEach(_FunctionT* function) {
		_foreach_function<_FunctionT> foreach_part(function);
		if (_parallel.max_thread_count() == 1)
			do_it(foreach_part,_collection);
		else
			do_parallel(foreach_part,_collection);
		return *this;
	}

/*
	template <typename _FunctionT>
	stream& to_map(_FunctionT& function) {
		_foreach_function<_FunctionT> foreach_part(&function);
		collection_type dest_collection;
		if (_parallel.max_thread_count() == 1)
			do_it(foreach_part,dest_collection);
		else
			do_parallel(foreach_part,dest_collection);
		_collection = dest_collection;
		return *this;
	}
*/
	collection_type& collection() {
		return _collection;
	}

};

template <typename _CollectionT, typename _ParallelStrategy>
inline stream<_CollectionT, _ParallelStrategy> stream_of(const _CollectionT& _collection, _ParallelStrategy _parallel) {
	return stream<_CollectionT, _ParallelStrategy>(_collection, _parallel);
}

template <typename _CollectionT>
inline stream<_CollectionT, single_thread_parallel_strategy> stream_of(const _CollectionT& _collection) {
	return stream<_CollectionT, single_thread_parallel_strategy>(_collection, single_thread_parallel_strategy());
}

} // namespace containers

} // namespace utils

#endif /* INCLUDE_UTILS_CONTAINERS_STREAM_HPP_ */
