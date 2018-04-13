/*
 * thread_impl.hpp
 *
 *  Created on: 12/04/2010
 *      Author: alan.lohse
 */

#ifndef THREAD_IMPL_HPP_
#define THREAD_IMPL_HPP_

#include <utils/utils_defs.hpp>
#include <utils/thread/mutex.hpp>
#include <set>
#include <windows.h>
#include <process.h>

namespace utils {

namespace __win32_impl {

class thread_group_base;

class thread_base {
public:
	typedef HANDLE native_handle_type;
	typedef t_dword id_type;
protected:
	template <typename _Ft>
	static t_uint __stdcall _M_thread_proc(t_pointer param) {
		_Ft* f = reinterpret_cast<_Ft*>(param);
		(*f)();
		return 0;
	}
	static t_uint __stdcall _M_self_thread_proc(t_pointer param) {
		thread_base* f = reinterpret_cast<thread_base*>(param);
		(*f).run();
		return 0;
	}
	virtual void run(){};
protected:
	friend class thread_group_base;
	native_handle_type _intern_handle;
	id_type _id;
	thread_group_base* _group;
	typedef t_uint (__stdcall *_int_thread_proc)(t_pointer param);

	void _M_thread_create(_int_thread_proc thread_proc, t_pointer param) {
		_intern_handle = (native_handle_type)CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread_proc,param,CREATE_SUSPENDED,&_id);
	}

	static native_handle_type _M_thread_start(_int_thread_proc thread_proc, t_pointer param) {
		t_uint dwThreadId = 0;
		return (native_handle_type)_beginthreadex(NULL, 0, thread_proc, (void*)param, 0, &dwThreadId);
	}

	int compare(const thread_base& other) const {
		return (ptrdiff_t)_intern_handle - (ptrdiff_t)other._intern_handle;
	}

	void _swap_groups(thread_base& other);
	void _remove_group();
	void _add_group();

public:

	thread_base() : _intern_handle(NULL), _id(0), _group(NULL) {
	}

	thread_base(thread_group_base* group) : _intern_handle(NULL), _id(0), _group(group) {
		if (_group)
			_add_group();
	}

	virtual ~thread_base() {
		if (joinable()) {
			join();
			if (_intern_handle) CloseHandle(_intern_handle);
		}
		if (_group)
			_remove_group();
	}

	native_handle_type native_handle() const {
		return _intern_handle;
	}

	id_type id() const {
		return _id;
	}

	void exit(t_int code) {
		ExitThread(code);
	}

	bool suspend() {
		return joinable() && SuspendThread((HANDLE)_intern_handle) != 0xFFFFFFFF;
	}

	bool resume() {
		return joinable() && ResumeThread((HANDLE)_intern_handle) != 0xFFFFFFFF;
	}

	bool terminate() {
		return TerminateThread((HANDLE)_intern_handle,0x103);
	}

	bool join() {
		return _intern_handle && WaitForSingleObject((HANDLE)_intern_handle, INFINITE) != 0xFFFFFFFF;
	}

	bool join(t_int timeout) {
		 return _intern_handle && WaitForSingleObject((HANDLE)_intern_handle,timeout < 0 ? INFINITE : timeout) != WAIT_FAILED;
	}

	bool joinable() const throw() {
		return _intern_handle && _intern_handle != (native_handle_type)GetCurrentThread();
	}

	bool set_priority(t_int priority) {
		int wPrior = 0;
		switch(priority) {
		case -3: wPrior = THREAD_PRIORITY_IDLE; break;
		case -2: wPrior = THREAD_PRIORITY_LOWEST; break;
		case -1: wPrior =  THREAD_PRIORITY_BELOW_NORMAL; break;
		case 0: wPrior = THREAD_PRIORITY_NORMAL; break;
		case 1: wPrior = THREAD_PRIORITY_ABOVE_NORMAL; break;
		case 2: wPrior = THREAD_PRIORITY_HIGHEST; break;
		case 3: wPrior = THREAD_PRIORITY_TIME_CRITICAL; break;
		default: return false;
		}
		return SetThreadPriority((HANDLE)_intern_handle,wPrior);
	}

	t_int get_priority() const {
		int priority = GetThreadPriority((HANDLE)_intern_handle);
		switch(priority) {
		case THREAD_PRIORITY_IDLE: return -3;
		case THREAD_PRIORITY_LOWEST: return -2;
		case THREAD_PRIORITY_BELOW_NORMAL: return -1;
		case THREAD_PRIORITY_NORMAL: return 0;
		case THREAD_PRIORITY_ABOVE_NORMAL: return 1;
		case THREAD_PRIORITY_HIGHEST: return 2;
		case THREAD_PRIORITY_TIME_CRITICAL: return 3;
		}
		return -0x7FFFFFFF;
	}

	static native_handle_type get_current() {
		return (native_handle_type)GetCurrentThread();
	}

	static t_int get_current_thread_id() {
		return GetCurrentThreadId();
	}

	static void yield() {
		Sleep(0);
	}

	static void sleep(t_int time) {
		Sleep(time);
	}

	bool operator == (const thread_base& other) const {
		return compare(other) == 0;
	}

	bool operator < (const thread_base& other) const {
		return compare(other) < 0;
	}

};

class thread_group_base {
protected:
	std::set<thread_base*> _threads;
	Mutex _mutex;
	friend class thread_base;
	void add_thread(thread_base* _thread) {
		_mutex.lock();
		_threads.insert(_thread);
		_thread->_group = this;
		_mutex.unlock();
	}
	void rem_thread(thread_base* _thread) {
		_mutex.lock();
		std::set<thread_base*>::iterator it = _threads.find(_thread);
		if (it != _threads.end()) {
			_thread->_group = NULL;
			_threads.erase(it);
		}
		_mutex.unlock();
	}
	void rem_add(thread_base* t1,thread_base* t2) {
		_mutex.lock();
		if (_threads.erase(t1) != 0)
			_threads.insert(t2);
		_mutex.unlock();
	}
	void clear() {
		_mutex.lock();
		std::set<thread_base*>::iterator it = _threads.begin();
		std::set<thread_base*>::iterator end = _threads.end();
		for (; it != end; ++it) {
			thread_base* thd = (thread_base*)(*it);
			thd->_group = NULL;
		}
		_threads.clear();
		_mutex.unlock();
	}

public:
	thread_group_base() : _threads() {
	}

	virtual ~thread_group_base() {
		clear();
	}

	void wait_all(t_dword timeout = INFINITE) {
		HANDLE phThreads[64];
		_mutex.lock();
		DWORD n = 0;
		std::set<thread_base*>::iterator it = _threads.begin();
		std::set<thread_base*>::iterator end = _threads.end();
		for (; it != end; ++it)
			phThreads[n++] = (*it)->_intern_handle;
		_mutex.unlock();
		::WaitForMultipleObjects(n,phThreads,1,timeout);
	}

	void wait_any(t_dword timeout = INFINITE) {
		HANDLE phThreads[64];
		_mutex.lock();
		DWORD n = 0;
		std::set<thread_base*>::iterator it = _threads.begin();
		std::set<thread_base*>::iterator end = _threads.end();
		for (; it != end; ++it)
			phThreads[n++] = (*it)->_intern_handle;
		_mutex.unlock();
		::WaitForMultipleObjects(n,phThreads,0,timeout);
	}

};

inline void thread_base::_swap_groups(thread_base& other) {
	if (_group) _group->rem_add(this,&other);
	if (other._group) other._group->rem_add(&other,this);
}

inline void thread_base::_remove_group() {
	_group->rem_thread(this);
}

inline void thread_base::_add_group() {
	_group->add_thread(this);
}


}

}

#endif /* THREAD_IMPL_HPP_ */
