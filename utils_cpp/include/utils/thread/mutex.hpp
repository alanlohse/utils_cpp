/*
 * mutex.hpp
 *
 *  Created on: 24/09/2009
 *      Author: alanlohse
 */

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include <utils/utils_defs.hpp>
#ifdef WINDOWS_THREAD_IMPL
#include <windows.h>
#endif
#ifdef PTHREAD_IMPL
	#include <pthread.h>
#endif

namespace utils {

class Mutex {
private:
	bool initalized, locked;
#ifdef WINDOWS_THREAD_IMPL
	::CRITICAL_SECTION crit;
#endif
#ifdef PTHREAD_IMPL
	pthread_mutex_t mutex;
#endif
	void initialize() {
		initalized = true;
		locked = false;
#ifdef WINDOWS_THREAD_IMPL
		InitializeCriticalSection(&crit);
#endif
#ifdef PTHREAD_IMPL
		pthread_mutex_init (&mutex, NULL);
#endif
	}
public:
	Mutex() {
		initalized = false;
		locked = false;
	}

	~Mutex() {
		if (initalized) {
#ifdef WINDOWS_THREAD_IMPL
			DeleteCriticalSection(&crit);
#endif
#ifdef PTHREAD_IMPL
			pthread_mutex_destroy (&mutex);
#endif
		}
	}

	void lock() {
		if (!initalized)
			initialize();
#ifdef WINDOWS_THREAD_IMPL
		EnterCriticalSection(&crit);
#endif
#ifdef PTHREAD_IMPL
		pthread_mutex_lock (&mutex);
#endif
		locked = true;
	}

	void try_lock() {
		if (!initalized)
			return;
#ifdef WINDOWS_THREAD_IMPL
		TryEnterCriticalSection(&crit);
#endif
#ifdef PTHREAD_IMPL
		pthread_mutex_trylock(&mutex);
#endif
		locked = true;
	}

	void unlock() {
		if (!initalized)
			return;
		locked = false;
#ifdef WINDOWS_THREAD_IMPL
		LeaveCriticalSection(&crit);
#endif
#ifdef PTHREAD_IMPL
		pthread_mutex_unlock (&mutex);
#endif
	}

	bool is_locked() {
//		pthread_mutexattr_init(&attr);
//		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
//		pthread_mutex_init(&errchkmutex, &attr);
		return locked;

	}
};

}

#endif /* MUTEX_HPP_ */
