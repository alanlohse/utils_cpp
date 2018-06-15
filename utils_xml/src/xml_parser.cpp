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

using namespace utils::xml;

template<typename _CharT>
struct xml_defs {
	static _CharT equal_to() { return '='; }
	static _CharT new_line() { return '\n'; }
	static _CharT begin_tag() { return '<'; }
	static _CharT end_tag() { return '>'; }
	static _CharT bar() { return '/'; }
	static _CharT quote() { return '"'; }
	static _CharT colon() { return ':'; }
	static const _CharT* begin_cdata() { return "<![CDATA["; }
	static const _CharT* end_cdata() { return "]]>"; }
	static const _CharT* begin_doctype() { return "<!DOCTYPE"; }
	static const _CharT* end_doctype() { return ">"; }
	static const _CharT* begin_comment() { return "<!--"; }
	static const _CharT* end_comment() { return "-->"; }
	static int strcmp(const _CharT* s1, const _CharT* s2) {
		return ::strcmp(s1,s2);
	}
	static int strncmp(const _CharT* s1, const _CharT* s2, size_t n) {
		return ::strncmp(s1,s2,n);
	}
};

template<>
struct xml_defs<wchar_t> {
	static wchar_t equal_to() { return L'='; }
	static wchar_t new_line() { return L'\n'; }
	static wchar_t begin_tag() { return L'<'; }
	static wchar_t end_tag() { return L'>'; }
	static wchar_t bar() { return L'/'; }
	static wchar_t quote() { return L'"'; }
	static wchar_t colon() { return L':'; }
	static const wchar_t* begin_cdata() { return L"<![CDATA["; }
	static const wchar_t* end_cdata() { return L"]]>"; }
	static const wchar_t* begin_doctype() { return L"<!DOCTYPE"; }
	static const wchar_t* end_doctype() { return L">"; }
	static const wchar_t* begin_comment() { return L"<!--"; }
	static const wchar_t* end_comment() { return L"-->"; }
	static int strcmp(const wchar_t* s1, const wchar_t* s2) {
		return ::wcscmp(s1,s2);
	}
	static int strncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
		return ::wcsncmp(s1,s2,n);
	}
};

void basic_xml_parser<char>::parse(const std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {

}

void basic_xml_parser<wchar_t>::parse(const std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {

}
