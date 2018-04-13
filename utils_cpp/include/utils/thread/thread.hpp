/*
 * thread.hpp
 *
 *  Created on: 12/04/2010
 *      Author: alan.lohse
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>

#if defined(WINDOWS_THREAD_IMPL)
#include <utils/thread/win32_impl/thread_impl.hpp>
#elif defined(PTHREAD_IMPL)
#include <utils/thread/pthread_impl/thread_impl.hpp>
#endif

namespace utils {

#if defined(WINDOWS_THREAD_IMPL)
#define _thread_impl_ __win32_impl
#elif defined(PTHREAD_IMPL)
#define _thread_impl_ __pthread_impl
#endif

class thread_group;

class thread : public _thread_impl_::thread_base {
public:
	typedef t_uint (__stdcall *t_thread_proc)(t_pointer param);

	thread() {
		_M_thread_create(&_M_self_thread_proc, this);
	}

	thread(thread_group& group) : thread_base((_thread_impl_::thread_group_base*)&group) {
		_M_thread_create(&_M_self_thread_proc, this);
	}

	virtual ~thread() {
	}

	template <typename _Ft>
	thread(_Ft& function) {
		_M_thread_create(&_M_thread_proc<_Ft>, (t_pointer)&function);
	}

	template <typename _Ft>
	thread(thread_group& group, _Ft& function) : thread_base((_thread_impl_::thread_group_base*)&group) {
		_M_thread_create(&_M_thread_proc<_Ft>, (t_pointer)&function);
	}

	template <typename _Ft>
	thread(_Ft* function) {
		_M_thread_create(&_M_thread_proc<_Ft>, (t_pointer)function);
	}

	template <typename _Ft>
	thread(thread_group& group, _Ft* function) : thread_base((_thread_impl_::thread_group_base*)&group) {
		_M_thread_create(&_M_thread_proc<_Ft>, (t_pointer)function);
	}

	thread(t_thread_proc function, t_pointer param) {
		_M_thread_create((_int_thread_proc)function, param);
	}

	thread(thread_group& group, t_thread_proc function, t_pointer param) : thread_base((_thread_impl_::thread_group_base*)&group) {
		_M_thread_create((_int_thread_proc)function, param);
	}

	thread(thread& other) {
		swap(other);
	}

	void start() {
		resume();
	}

	bool operator == (const thread& other) const {
		return compare(other) == 0;
	}

	bool operator < (const thread& other) const {
		return compare(other) < 0;
	}

	thread& operator = (thread& other) {
		swap(other);
		return *this;
	}

	void swap(thread& other) {
		utils::swap(_intern_handle,other._intern_handle);
		_swap_groups(other);
		utils::swap(_group,other._group);
	}

	static native_handle_type start(t_thread_proc function, t_pointer param) {
		return _M_thread_start((_int_thread_proc)function, param);
	}

};

template <>
inline void swap<thread>(thread& t1, thread& t2) {
	t1.swap(t2);
}

class thread_group : public _thread_impl_::thread_group_base {
public:
	thread_group() : thread_group_base() {
	}

	virtual ~thread_group() {
	}

	size_t count() const {
		return _threads.size();
	}

};

}

#endif /* THREAD_HPP_ */
