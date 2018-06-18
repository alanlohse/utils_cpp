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

using namespace utils::xml;

template<typename _CharT>
struct xml_defs {
	typedef _CharT char_type;
	static char_type equal_to() { return '='; }
	static char_type new_line() { return '\n'; }
	static char_type begin_tag() { return '<'; }
	static char_type end_tag() { return '>'; }
	static char_type bar() { return '/'; }
	static char_type quote() { return '"'; }
	static char_type colon() { return ':'; }
	static char_type ampersand() { return '&'; }
	static char_type semicolon() { return ';'; }
	static const char_type* begin_cdata() { return "<![CDATA["; }
	static const char_type* end_cdata() { return "]]>"; }
	static const char_type* begin_doctype() { return "<!DOCTYPE"; }
	static const char_type* end_doctype() { return ">"; }
	static const char_type* begin_comment() { return "<!--"; }
	static const char_type* end_comment() { return "-->"; }
	static const char_type* begin_xml_decl() { return "<?xml"; }
	static const char_type* end_xml_decl() { return "?>"; }
	static const char_type* version() { return "version"; }
	static const char_type* encoding() { return "encoding"; }
	static const char_type* standalone() { return "standalone"; }
	static const char_type* element() { return "ELEMENT"; }
	static const char_type* pcdata() { return "PCDATA"; }
	static const char_type* cdata() { return "CDATA"; }
	static const char_type* empty() { return "EMPTY"; }
	static const char_type* attlist() { return "ATTLIST"; }
	static const char_type* entity() { return "ENTITY"; }
	static int strcmp(const char_type* s1, const char_type* s2) {
		return ::strcmp(s1,s2);
	}
	static int strncmp(const char_type* s1, const char_type* s2, size_t n) {
		return ::strncmp(s1,s2,n);
	}
};

template<>
struct xml_defs<wchar_t> {
	typedef wchar_t char_type;
	static wchar_t equal_to() { return L'='; }
	static wchar_t new_line() { return L'\n'; }
	static wchar_t begin_tag() { return L'<'; }
	static wchar_t end_tag() { return L'>'; }
	static wchar_t bar() { return L'/'; }
	static wchar_t quote() { return L'"'; }
	static wchar_t colon() { return L':'; }
	static wchar_t begin_xml_decl() { return L"<?xml"; }
	static wchar_t end_xml_decl() { return L"?>"; }
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
