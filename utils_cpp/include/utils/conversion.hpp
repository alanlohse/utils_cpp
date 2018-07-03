/*
 * conversion.hpp
 *
 *  Created on: 3 de jul de 2018
 *      Author: alan.lohse
 */

#ifndef INCLUDE_UTILS_CONVERSION_HPP_
#define INCLUDE_UTILS_CONVERSION_HPP_

#include <utils/utils_defs.hpp>
#include <string>
#include <cstdlib>
#include <cstring>

namespace utils {

/**
 * Converts from string into a value of the output type
 */
template <typename _OutputType, typename _StringType>
inline _OutputType from_string(const _StringType& str) {
	return _OutputType();
}

template <>
inline int from_string<int,std::string>(const std::string& str) {
	return atoi(str.c_str());
}

template <>
inline unsigned int from_string<unsigned int,std::string>(const std::string& str) {
	char* endp;
	return strtoul(str.c_str(), &endp, 10);
}

template <>
inline __int64 from_string<__int64,std::string>(const std::string& str) {
	return atoll(str.c_str());
}

template <>
inline unsigned __int64 from_string<unsigned __int64,std::string>(const std::string& str) {
	char* endp;
	return strtoull(str.c_str(), &endp, 10);
}

template <>
inline float from_string<float,std::string>(const std::string& str) {
	char* tail;
	return strtof(str.c_str(), &tail);
}

template <>
inline double from_string<double,std::string>(const std::string& str) {
	return atof(str.c_str());
}

typedef char* c_str_t;
typedef wchar_t* wc_str_t;

template <>
inline int from_string<int,c_str_t>(const c_str_t& str) {
	return atoi(str);
}

template <>
inline unsigned int from_string<unsigned int,c_str_t>(const c_str_t& str) {
	c_str_t endp;
	return strtoul(str, &endp, 10);
}

template <>
inline __int64 from_string<__int64,c_str_t>(const c_str_t& str) {
	return atoll(str);
}

template <>
inline unsigned __int64 from_string<unsigned __int64,c_str_t>(const c_str_t& str) {
	c_str_t endp;
	return strtoull(str, &endp, 10);
}

template <>
inline float from_string<float,c_str_t>(const c_str_t& str) {
	c_str_t tail;
	return strtof(str, &tail);
}

template <>
inline double from_string<double,c_str_t>(const c_str_t& str) {
	return atof(str);
}


template <>
inline int from_string<int,std::wstring>(const std::wstring& str) {
	return _wtoi(str.c_str());
}

template <>
inline unsigned int from_string<unsigned int,std::wstring>(const std::wstring& str) {
	wchar_t* endp;
	return wcstoul(str.c_str(), &endp, 10);
}

template <>
inline __int64 from_string<__int64,std::wstring>(const std::wstring& str) {
	wchar_t* endp;
	return wcstoll(str.c_str(),&endp,10);
}

template <>
inline unsigned __int64 from_string<unsigned __int64,std::wstring>(const std::wstring& str) {
	wchar_t* endp;
	return wcstoull(str.c_str(), &endp, 10);
}

template <>
inline float from_string<float,std::wstring>(const std::wstring& str) {
	wchar_t* tail;
	return wcstof(str.c_str(), &tail);
}

template <>
inline double from_string<double,std::wstring>(const std::wstring& str) {
	return _wtof(str.c_str());
}


template <>
inline int from_string<int,wc_str_t>(const wc_str_t& str) {
	return _wtoi(str);
}

template <>
inline unsigned int from_string<unsigned int,wc_str_t>(const wc_str_t& str) {
	wc_str_t endp;
	return wcstoul(str, &endp, 10);
}

template <>
inline __int64 from_string<__int64,wc_str_t>(const wc_str_t& str) {
	wc_str_t endp;
	return wcstoll(str, &endp, 10);
}

template <>
inline unsigned __int64 from_string<unsigned __int64,wc_str_t>(const wc_str_t& str) {
	wc_str_t endp;
	return wcstoull(str, &endp, 10);
}

template <>
inline float from_string<float,wc_str_t>(const wc_str_t& str) {
	wc_str_t tail;
	return wcstof(str, &tail);
}

template <>
inline double from_string<double,wc_str_t>(const wc_str_t& str) {
	return _wtof(str);
}


/**
 * Converts from a value of the input type into a string
 */
template <typename _InputType, typename _StringType>
inline _StringType to_string(_InputType val) {
	return _StringType(val);
}

template <>
inline std::string to_string<int,std::string>(int val) {
	char buf[16];
	return std::string(itoa(val,buf,10));
}

template <>
inline std::string to_string<unsigned long int,std::string>(unsigned long int val) {
	char buf[16];
	return std::string(ultoa(val,buf,10));
}

template <>
inline std::string to_string<__int64,std::string>(__int64 val) {
	char buf[24];
	return std::string(lltoa(val,buf,10));
}

template <>
inline std::string to_string<unsigned __int64,std::string>(unsigned __int64 val) {
	char buf[24];
	return std::string(ulltoa(val,buf,10));
}

template <>
inline std::string to_string<float,std::string>(float val) {
	char buf[16];
	return std::string(gcvt(val,7,buf));
}

template <>
inline std::string to_string<double,std::string>(double val) {
	char buf[16];
	return std::string(gcvt(val,15,buf));
}


} // namespace utils


#endif /* INCLUDE_UTILS_CONVERSION_HPP_ */
