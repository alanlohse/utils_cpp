/*
 * xml_parser.cpp
 *
 *  Created on: 15 de jun de 2018
 *      Author: alan.lohse
 */

#include <utils/xml/xml_parser.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <memory>

using namespace utils::xml;

#define XC_EQUALS 0
#define XC_RETURN 1
#define XC_LT 2
#define XC_GT 3
#define XC_SLASH 4
#define XC_QUOTE 5
#define XC_COLON 6
#define XC_AMP 7
#define XC_SEMICOLON 8
#define XC_BEGIN_CDATA 9
#define XC_END_CDATA 10
#define XC_BEGIN_DOCTYPE 11
#define XC_END_DOCTYPE 12
#define XC_BEGIN_COMMENT 13
#define XC_END_COMMENT 14
#define XC_BEGIN_XML_DECL 15
#define XC_END_XML_DECL 16
#define XC_VERSION 17
#define XC_ECODING 18
#define XC_STANDALONE 19
#define XC_ELEMENT 20
#define XC_PCDATA 21
#define XC_CDATA 22
#define XC_EMPTY 23
#define XC_ATTLIST 24
#define XC_ENTITY 25
#define XC_REQUIRED 26
#define XC_IMPLIED 27
#define XC_OPEN_PAR 28
#define XC_CLOSE_PAR 29
#define XC_PIPE 30
#define XC_PLUS 31
#define XC_QUESTION 32
#define XC_FIXED 33
#define XC_ASTERIX 34
#define XC_ANY 35
#define XC_OPEN_BRAC 36
#define XC_CLOSE_BRAC 37

const char* _char_xml_constants[] = {
		"=", "\n", "<", ">", "/", "\"", ":", "&",
		";", "<![CDATA[", "]]>", "<!DOCTYPE", ">", "<!--", "-->", "<?xml", "?>",
		"version", "encoding", "standalone", "<!ELEMENT", "#PCDATA", "CDATA",
		"EMPTY", "<!ATTLIST", "<!ENTITY", "#REQUIRED", "#IMPLIED", "(", ")",
		"|", "+", "?", "#FIXED", "*", "ANY", "[", "]"
};

template<typename _CharT>
struct xml_defs {
	typedef _CharT char_type;
	static const char_type* text(int index) { return _char_xml_constants[index]; }
	static int strcmp(const char_type* s1, const char_type* s2) {
		return ::strcmp(s1,s2);
	}
	static int strncmp(const char_type* s1, const char_type* s2, size_t n) {
		return ::strncmp(s1,s2,n);
	}
};

const wchar_t* _wchar_xml_constants[] = {
		L"=", L"\n", L"<", L">", L"/", L"\"",
		L":", L"&", L";", L"<![CDATA[", L"]]>", L"<!DOCTYPE", L">", L"<!--",
		L"-->", L"<?xml", L"?>", L"version", L"encoding", L"standalone",
		L"<!ELEMENT", L"#PCDATA", L"CDATA", L"EMPTY", L"<!ATTLIST", L"<!ENTITY",
		L"#REQUIRED", L"#IMPLIED", L"(", L")", L"|", L"+", L"?", L"#FIXED",
		L"*", L"ANY", L"[", L"]"
};

template<>
struct xml_defs<wchar_t> {
	typedef wchar_t char_type;
	static const char_type* text(int index) { return _wchar_xml_constants[index]; }
	static int strcmp(const wchar_t* s1, const wchar_t* s2) {
		return ::wcscmp(s1,s2);
	}
	static int strncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
		return ::wcsncmp(s1,s2,n);
	}
};

template <typename _CharT, class _Alloc>
class string_builder {
public:
	typedef _CharT char_type;
	typedef typename _Alloc::template rebind<char_type>::other allocator_type;
	typedef size_t size_type;
private:
	char_type* _M_begin;
	char_type* _M_cur;
	char_type* _M_end;
	allocator_type _allocator;
	void _resize(size_type n) {
		if (n == 0) return;
		char_type* old_begin = _M_begin;
		char_type* old_cur = _M_cur;
		char_type* old_end = _M_end;
		_M_begin = _allocator.allocate(n);
		_M_cur = _M_begin;
		_M_end = _M_begin + n;
		*_M_cur = 0;
		if (old_begin) {
			for (char_type* cur = old_begin; cur < old_cur; cur++, _M_cur++)
				*_M_cur = *cur;
			*_M_cur = 0;
		}
	}
public:
	string_builder(size_type n = 0) : _M_begin(nullptr),
		_M_cur(nullptr),
		_M_end(nullptr),
		_allocator() {
		_resize(n);
	}
	~string_builder() {
		if (_M_begin)
			_allocator.deallocate(_M_begin, _M_end - _M_begin);
	}
	size_type length() const {
		return _M_cur - _M_begin;
	}
	string_builder& push(char_type ch) {
		if (_M_cur == _M_end - 1) {
			_resize(length() * 2);
		}
		*(_M_cur++) = ch;
		*_M_cur = 0;
		return *this;
	}
	string_builder& clear() {
		_M_cur = _M_begin;
		*_M_cur = 0;
		return *this;
	}
	char_type* c_str() const {
		return _M_begin;
	}
};

template<typename _CharT>
void parse(const std::basic_istream<_CharT>& is, tag_handler<_CharT>* _handler) {
	string_builder<_CharT,std::allocator<_CharT>> builder;
	while(is) {
		_CharT ch = (_CharT) is.get();
		builder.push(ch);
	}
}

void basic_xml_parser<char>::parse(const std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {

}

void basic_xml_parser<wchar_t>::parse(const std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {

}
