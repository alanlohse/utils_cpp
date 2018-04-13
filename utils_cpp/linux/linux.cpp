/*
 * linux.cpp
 *
 *  Created on: 18 de jan de 2016
 *      Author: alan
 */
#ifdef UTILS_LINUX

#include <cstring>
#include <ctype.h>
#include <time.h>
#include <utils/time.hpp>

char * itoa(int value, char * str, int base) {
	int v = value;
	char buf[33];
	char * ds = buf + 32;
	*ds = 0;
	for (; v && ds > buf; --ds, v /= base)
		*ds = "0123456789abcdef"[v % base];
	if (base == 0 && (value & 0x80000000) != 0)
		*(ds--) = '-';
	strcpy(str,ds++);
	return str;
}

char *strlwr(char *str) {
	unsigned char *p = (unsigned char *) str;

	while (*p) {
		*p = tolower(*p);
		p++;
	}

	return str;
}

namespace utils {


t_bigint Time::nanoseconds() {
	t_bigint time = 0;
	struct timespec tp;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
	time = (t_bigint)tp.tv_sec * 1000000000LL + tp.tv_nsec;
	return time;
}

t_bigint Time::zeroTime() {
	return 0;
}

t_bigint Time::milliseconds() {
	t_bigint time = 0;
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	time = (t_bigint)tp.tv_sec * 1000LL + tp.tv_nsec / 1000000LL;
	return time;
}

}

#endif
