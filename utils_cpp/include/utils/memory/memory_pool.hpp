/*
 * memory_pool.hpp
 *
 *  Created on: 24/09/2009
 *      Author: alanlohse
 */

#ifndef MEMORY_POOL_HPP_
#define MEMORY_POOL_HPP_

#include <utils/utils_defs.hpp>
#include <utils/containers/iterators.hpp>
#include <utils/funcs.hpp>
#include <utils/thread/mutex.hpp>
#include <utils/time.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <new>

namespace utils {

namespace memory {

struct default_growth_policy {
	size_t starter_memory_size;
	size_t min_block_size;
	size_t max_memory_size;
	size_t starter_fragmented_size;
	size_t total_size;
	size_t total_free;
	size_t total_alloc;
	default_growth_policy() {
		starter_memory_size = 262144;
		min_block_size = 262144;
		max_memory_size = 0;
		starter_fragmented_size = 4096;
		total_size = 0;
		total_free = 0;
		total_alloc = 0;
	}

	size_t get_min_fragmented_size() {
		return 4;
	}
	size_t get_max_fragmented_size() {
		return 4096;
	}
	size_t get_fragmentation_slot_count() {
		return 4092;
	}
	bool can_alloc(size_t size) {
		return max_memory_size == 0 || max_memory_size >= total_size + size;
	}
	void add_mem(size_t size) {
		total_size += size;
	}
	void add_free_mem(size_t size) {
		total_free += size;
	}
	void sub_mem(size_t size) {
		total_size -= size;
	}
	void sub_free_mem(size_t size) {
		total_free -= size;
	}
	void add_alloc_mem(size_t size) {
		total_alloc += size;
	}
	void sub_alloc_mem(size_t size) {
		total_alloc -= size;
	}
	size_t get_total_size() {
		return total_size;
	}
	size_t get_total_free() {
		return total_free;
	}
	size_t get_total_alloc() {
		return total_alloc;
	}
};

struct default_malloc {
	void* mem_alloc(size_t size) {
		return ::malloc(size);
	}
	void mem_free(void* mem) {
		return ::free(mem);
	}
};

template<typename _MutexT = Mutex, typename _GrowthPolicyT = default_growth_policy, typename _MallocT = default_malloc>
class memory_pool {
public:
	typedef memory_pool<_MutexT,_GrowthPolicyT,_MallocT> memory_pool_type;
	typedef _MutexT			mutex_type;
	typedef _GrowthPolicyT	growth_policy_type;
	typedef _MallocT		malloc_type;
private:

	struct memory_slot {
		memory_slot* next;
		memory_slot(memory_slot* _next): next(_next) {
		}
		void* base() {
			return (void*) (((size_t) this) + sizeof(memory_slot));
		}
		static memory_slot* to_slot(void* base) {
			return (memory_slot*) (((size_t) base) - sizeof(memory_slot));
		}
	};
	struct slot_group {
		size_t size;
		slot_group* next;
		memory_slot* free_slots;
		slot_group(size_t _size, slot_group* _next, memory_slot* _free_slots):
			size(_size),
			next(_next),
			free_slots(_free_slots) {
		}
		memory_slot* pop_free() {
			if (free_slots) {
				memory_slot* slot = free_slots;
				free_slots = slot->next;
				slot->next = NULL;
				return slot;
			}
			return NULL;
		}
		void push_free(memory_slot* slot) {
			slot->next = free_slots;
			free_slots = slot;
		}
	};
	struct _next_group {
		slot_group* operator()(slot_group* _v) {
			return _v->next;
		}
	};
	typedef iterators::forward_list_iterator<slot_group,_next_group> group_iterator;
	struct memory_map {
		slot_group* groups[2027];
		memory_map() {
			for (int i = 0; i < 2027; i++) {
				groups[i] = NULL;
			}
		}
		void put(slot_group* group) {
			int p = group->size % 2027;
			slot_group* _prev_group = groups[p];
			if (_prev_group)
				group->next = _prev_group;
			groups[p] = group;
		}
		slot_group* get(size_t size) {
			int p = size % 2027;
			slot_group* group = groups[p];
			group_iterator it  = group;
			group_iterator end = NULL;
			for (; it != end; ++it) {
				if (it->size == size)
					return it._M_cur;
			}
			return NULL;
		}
	};

	memory_slot** free_slots;
	memory_map big_slots;
	mutex_type mutex;
	growth_policy_type growth_policy;
	malloc_type mem_alloc;

	memory_pool() :
		big_slots(),
		mutex(),
		growth_policy(),
		mem_alloc() {
		free_slots = new memory_slot*[growth_policy.get_fragmentation_slot_count()];
		for (size_t i = 0; i < growth_policy.get_fragmentation_slot_count(); i++) {
			free_slots[i] = NULL;
		}
	}
	memory_slot* alloc_slot(size_t size, memory_slot* next) {
		size_t effc_size = size + sizeof(memory_slot);
		if (growth_policy.can_alloc(effc_size)) {
			void* m = ::malloc(effc_size);
			if (m) {
				growth_policy.add_mem(effc_size);
				return ::new (m) memory_slot(next);
			}
		}
		return NULL;
	}
public:
	~memory_pool() {

	}

	void* malloc(size_t size) {
		if (growth_policy.get_min_fragmented_size() && growth_policy.get_min_fragmented_size()> size)
			return NULL;
		void* mem = NULL;
		mutex.lock();
		if (growth_policy.can_alloc(size)) {
			if (size <= growth_policy.get_max_fragmented_size()) {
				int p = size - growth_policy.get_min_fragmented_size();
				memory_slot* slot = free_slots[p];
				if (!slot) {
					slot = alloc_slot(size,NULL);
				} else {
					free_slots[p] = slot->next;
					growth_policy.sub_free_mem(size);
				}
				if (slot) {
					growth_policy.add_alloc_mem(size);
					mem = slot->base();
				}
			} else {
				slot_group* group = big_slots.get(size);
				memory_slot* slot = NULL;
				if (!group) {
					slot = alloc_slot(size,NULL);
					group = new slot_group(size, NULL, NULL);
					big_slots.put(group);
				} else {
					slot = group->pop_free();
					if (!slot)
						slot = alloc_slot(size,NULL);
					else
						growth_policy.sub_free_mem(size);
				}
				if (slot) {
					growth_policy.add_alloc_mem(size);
					mem = slot->base();
				}
			}
		}
		mutex.unlock();
		return mem;
	}

	void free(void* base, size_t size) {
		mutex.lock();
		if (size <= growth_policy.get_max_fragmented_size()) {
			int p = size - growth_policy.get_min_fragmented_size();
			memory_slot* slot = memory_slot::to_slot(base);
			slot->next = free_slots[p];
			free_slots[p] = slot;
		} else {
			slot_group* group = big_slots.get(size);
			memory_slot* slot = memory_slot::to_slot(base);
			group->push_free(slot);
		}
		growth_policy.sub_alloc_mem(size);
		growth_policy.add_free_mem(size);
		mutex.unlock();
	}

	size_t get_free_memory() {
		return growth_policy.get_total_free();
	}

	size_t get_used_memory() {
		return growth_policy.get_total_alloc();
	}

	size_t get_total_memory() {
		return growth_policy.get_total_size();
	}

	static memory_pool_type& get_instance() {
		static memory_pool_type instance;
		return instance;
	}

};

template<typename _Tp, typename _MutexT = Mutex, typename _GrowthPolicyT = default_growth_policy, typename _MallocT = default_malloc>
class pooled_allocator {
	typedef memory_pool<_MutexT,_GrowthPolicyT,_MallocT> memory_pool_type;
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Tp* pointer;
	typedef const _Tp* const_pointer;
	typedef _Tp& reference;
	typedef const _Tp& const_reference;
	typedef _Tp value_type;

	template<typename _Tp1>
	struct rebind {
		typedef pooled_allocator<_Tp1> other;
	};

	pooled_allocator() throw () {
	}

	pooled_allocator(const pooled_allocator& other) throw () {
	}

	template<typename _Tp2>
	pooled_allocator(const _Tp2&) throw () {
	}

	~pooled_allocator() throw () {
	}

	pointer address(reference __x) const {
		return &__x;
	}

	const_pointer address(const_reference __x) const {
		return &__x;
	}

	pointer allocate(size_type __n, const void* = 0) {
		void* mem = memory_pool_type::get_instance().malloc(__n * sizeof(_Tp));
		return static_cast<_Tp*> (mem);
	}

	void deallocate(pointer __p, size_type __n) {
		memory_pool_type::get_instance().free(__p,__n * sizeof(_Tp));
	}

	size_type max_size() const throw () {
		return size_t(-1) / sizeof(_Tp);
	}

	void construct(pointer __p, const _Tp& __val) {
		::new (__p) _Tp(__val);
	}

	void destroy(pointer __p) {
		__p->~_Tp();
	}

};

template<typename _Tp,  typename _MutexT, typename _GrowthPolicyT, typename _MallocT>
inline bool operator==(const pooled_allocator<_Tp,_MutexT,_GrowthPolicyT,_MallocT>&,
		const pooled_allocator<_Tp,_MutexT,_GrowthPolicyT,_MallocT>&) {
	return true;
}

template<typename _Tp, typename _MutexT, typename _GrowthPolicyT, typename _MallocT>
inline bool operator!=(const pooled_allocator<_Tp,_MutexT,_GrowthPolicyT,_MallocT>&,
		const pooled_allocator<_Tp,_MutexT,_GrowthPolicyT,_MallocT>&) {
	return false;
}

struct null_growth_policy {
	null_growth_policy() {
	}

	size_t get_min_fragmented_size() {
		return 4;
	}
	size_t get_max_fragmented_size() {
		return 4096;
	}
	size_t get_fragmentation_slot_count() {
		return 4092;
	}
	bool can_alloc(size_t size) {
		return true;
	}
	void add_mem(size_t size) {
	}
	void add_free_mem(size_t size) {
	}
	void sub_mem(size_t size) {
	}
	void sub_free_mem(size_t size) {
	}
	void add_alloc_mem(size_t size) {
	}
	void sub_alloc_mem(size_t size) {
	}
	size_t get_total_size() {
		return 0;
	}
	size_t get_total_free() {
		return 0;
	}
	size_t get_total_alloc() {
		return 0;
	}
};

struct new_malloc {
	void* mem_alloc(size_t size) {
		return ::operator new(size);
	}
	void mem_free(void* mem) {
		return ::operator delete(mem);
	}
};

}

}

#endif /* MEMORY_POOL_HPP_ */
