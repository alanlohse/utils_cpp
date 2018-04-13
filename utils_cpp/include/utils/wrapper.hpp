/*
 * wrapper.hpp
 *
 *  Created on: 26/12/2009
 *      Author: Alan
 */

#ifndef WRAPPER_HPP_
#define WRAPPER_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/exception.hpp>

namespace utils {

template <typename T>
class Wrapper {
private:
	bool assigned;
	T value;
public:
	Wrapper():
		value(),
		assigned(false)
	{
	}

	Wrapper(const T& v):
		value(v),
		assigned(true)
	{
	}

	Wrapper(const Wrapper& other):
		value(other.value),
		assigned(other.assigned)
	{
	}

	bool isEmpty() const {
		return !assigned;
	}

	const Wrapper& operator = (const Wrapper& other) {
		value = other.value;
		assigned = other.assigned;
		return *this;
	}

	Wrapper& operator = (const T& other) {
		value = other;
		assigned = true;
		return *this;
	}

	T& get_value() const {
		return value;
	}

	T* operator -> () const {
		return &value;
	}

	T* operator & () const {
		return &value;
	}

	T& operator * () const {
		return value;
	}

};

template <typename T, class _Alloc = DEFAULT_ALLOCATOR<T>, typename _Mutex = null_mutex>
class SmartPointer {
public:
	typedef T base_type;
	typedef T* pointer_type;
	typedef T& reference_type;
	typedef _Alloc allocator_type;
	typedef ptrdiff_t reference_count_type;
	typedef _Mutex mutex_type;
	struct smart_pointer_data_type {
		pointer_type M_value;
		volatile reference_count_type M_refs;
		mutex_type M_mutex;
		allocator_type M_allocator;
		smart_pointer_data_type() : M_value(NULL), M_refs(0), M_mutex(), M_allocator() {}
		smart_pointer_data_type(const pointer_type& value, const reference_count_type& refs = 0,
				const mutex_type& mutex = mutex_type(), const allocator_type& allocator =allocator_type()) :
			M_value(value), M_refs(refs + 1), M_mutex(mutex), M_allocator(allocator) { }
		smart_pointer_data_type(const smart_pointer_data_type& other) :
			M_value(other.M_value), M_refs(other.M_refs + 1), M_mutex(other.M_mutex), M_allocator(other.M_allocator) { }
		~smart_pointer_data_type() {
		}
		smart_pointer_data_type* remove_ref() throw() {
			M_mutex.lock();
			if (M_value && --M_refs <= 0) {
				M_allocator.deallocate(M_value);
				M_value = NULL;
			}
			M_mutex.unlock();
			return this;
		}
		smart_pointer_data_type* add_ref() throw() {
			M_mutex.lock();
			if (M_value) {
				++M_refs;
			}
			M_mutex.unlock();
			return this;
		}
	};
	typedef typename allocator_type::template rebind<smart_pointer_data_type>::other allocator_data_type;
private:
	smart_pointer_data_type* M_data;
	allocator_data_type M_allocator_data;
	smart_pointer_data_type* increment_ref() {
		return M_data ? M_data->add_ref() : M_data;
	}
	void decrement_ref() {
		if (M_data)	{
			M_data->remove_ref();
			if (M_data->M_value == NULL) {
				M_allocator_data.destroy(M_data);
				M_allocator_data.deallocate(M_data,1);
			}
		}
	}
public:
	SmartPointer() : M_data(NULL), M_allocator_data() {
	}

	template<typename T2, class Alloc2, typename mutex2>
	explicit SmartPointer(const SmartPointer<T2,Alloc2,mutex2>& other) : M_data(other.increment_ref()), M_allocator_data(other.M_allocator) {
	}

	explicit SmartPointer(const pointer_type& ptr, const allocator_type& allocator) : M_data(NULL), M_allocator_data() {
		M_data = M_allocator_data.allocate(1);
		M_allocator_data.construct(M_data,smart_pointer_data_type(ptr,0,mutex_type(),allocator));
	}

	explicit SmartPointer(const pointer_type& ptr) : M_data(NULL), M_allocator_data() {
		M_data = M_allocator_data.allocate(1);
		M_allocator_data.construct(M_data,smart_pointer_data_type(ptr,0,mutex_type()));
	}

	~SmartPointer() {
		decrement_ref();
	}

	reference_type get_reference() const {
		if (M_data == NULL || M_data->M_value == NULL)
			throw utils_exception("Invalid reference.");
		return *M_data->M_value;
	}

	pointer_type get_pointer() const {
		if (M_data == NULL || M_data->M_value == NULL)
			throw utils_exception("Invalid reference.");
		return M_data->M_value;
	}


	reference_type operator * () const {
		return get_reference();
	}

	pointer_type operator & () const {
		return get_pointer();
	}

	pointer_type operator -> () const {
		return get_pointer();
	}

	SmartPointer& operator = (const SmartPointer& other) {
		decrement_ref();
		M_data = other.M_data ? other.M_data->add_ref() : NULL;
		M_allocator_data = other.M_allocator_data;
		return *this;
	}


};

}


#endif /* WRAPPER_HPP_ */
