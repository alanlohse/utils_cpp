/*
 * utils.cpp
 *
 *  Created on: 23/04/2011
 *      Author: Alan
 */

#include <utils/time.hpp>
#include <windows.h>

void* setWindowPointer(HWND hWnd, int index, void* value) {
#ifdef _WIN64
	return (void*) SetWindowLongPtr(hWnd, index, (LONG_PTR)value);
#else
	return (void*) SetWindowLongPtr(hWnd, index, (long int)value);
#endif
}

void* getWindowPointer(HWND hWnd, int index) {
	return (void*) GetWindowLongPtr(hWnd, index);
}

void* setClassPointer(HWND hWnd, int index, void* value) {
#ifdef _WIN64
	return (void*) SetClassLongPtr(hWnd, index, (LONG_PTR)value);
#else
	return (void*) SetClassLongPtr(hWnd, index, (long int)value);
#endif
}

void* getClassPointer(HWND hWnd, int index) {
	return (void*) GetClassLongPtr(hWnd, index);
}

