/*
 * utils.cpp
 *
 *  Created on: 23/04/2011
 *      Author: Alan
 */

#include <utils/time.hpp>
#include <windows.h>

namespace utils {


t_bigint Time::nanoseconds() {
	t_bigint time = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	return time;
}

t_bigint Time::zeroTime() {
	static t_bigint _offsetTime = 0;
	if (_offsetTime) return _offsetTime;
	SYSTEMTIME ori;
	ori.wYear          = 1970;
	ori.wMonth         = 1;
	ori.wDayOfWeek     = 0;
	ori.wDay           = 1;
	ori.wHour          = 0;
	ori.wMinute        = 0;
	ori.wSecond        = 0;
	ori.wMilliseconds  = 0;
	FILETIME ft;
	SystemTimeToFileTime(&ori, &ft);
	_offsetTime = ((LARGE_INTEGER*)(&ft))->QuadPart;
	return _offsetTime;
}

t_bigint Time::milliseconds() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return (*((t_bigint*)(&ft)) - zeroTime())/10000;
}

}
