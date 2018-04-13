/*
 * win32api_utils.h
 *
 *  Created on: 26/04/2015
 *      Author: Alan
 */

#ifndef WIN32API_UTILS_H_
#define WIN32API_UTILS_H_

#include <windows.h>

void* setWindowPointer(HWND hWnd, int index, void* value);

void* getWindowPointer(HWND hWnd, int index);

void* setClassPointer(HWND hWnd, int index, void* value);

void* getClassPointer(HWND hWnd, int index);

#define HAS_VALUE(x, y) ((x & y) == y)

#define TOGGLE_VALUE(x, y, on) { \
	bool is_on = (x & y) == y; \
	if (on && !is_on) \
		x |= y; \
	else if (!on && is_on) \
		x ^= y; \
}


#endif /* WIN32API_UTILS_H_ */
