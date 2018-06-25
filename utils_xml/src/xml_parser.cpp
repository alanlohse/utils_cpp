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
#define XC_ENCODING 18
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

std::regex IS_TAG("^<(\\w+)?[:]?(\\w+)\\s$");
std::wregex IS_TAGW(L"^<(\\w+)?[:]?(\\w+)\\s$");
std::regex ATTRS("(\\w+[:])?(\\w+)=\"(.*?)\"");
std::wregex ATTRSW(L"(\\w+[:])?(\\w+)=\"(.*?)\"");

template<typename _CharT>
struct xml_defs {
	typedef _CharT char_type;
	static const char_type* text(int index) { return _char_xml_constants[index]; }
	static const bool is_space(char_type ch) {
		return ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ';
	}
	static int strcmp(const char_type* s1, const char_type* s2) {
		return ::strcmp(s1,s2);
	}
	static int strlen(const char_type* s) {
		return ::strlen(s);
	}
	static int strncmp(const char_type* s1, const char_type* s2, size_t n) {
		return ::strncmp(s1,s2,n);
	}
	static bool endswith(const char_type* s1, const char_type* s2) {
		size_t ls1 = ::strlen(s1);
		size_t ls2 = ::strlen(s2);
		if (ls1 > ls2)
			return ::strcmp(s1 + ls1 - ls2, s2) == 0;
		else
			return ::strcmp(s2 + ls2 - ls1, s1) == 0;
	}
	static bool is_tag(const char_type* str) {
		return std::regex_match (str, IS_TAG);
	}
	template<typename _Found_proc>
	static void find_attr(const std::string& str, _Found_proc& found_proc) {
		auto words_begin =
		      std::sregex_iterator(str.begin(), str.end(), ATTRS);
		    auto words_end = std::sregex_iterator();

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		    std::smatch match = *i;
		    found_proc(match[1], match[2], match[3]);
		}
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
	static const bool is_space(char_type ch) {
		return ch == L'\n' || ch == L'\r' || ch == L'\t' || ch == L' ';
	}
	static int strlen(const char_type* s) {
		return ::wcslen(s);
	}
	static int strcmp(const wchar_t* s1, const wchar_t* s2) {
		return ::wcscmp(s1,s2);
	}
	static int strncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
		return ::wcsncmp(s1,s2,n);
	}
	static bool endswith(const char_type* s1, const char_type* s2) {
		size_t ls1 = wcslen(s1);
		size_t ls2 = wcslen(s2);
		if (ls1 > ls2)
			return ::wcscmp(s1 + ls1 - ls2, s2) == 0;
		else
			return ::wcscmp(s2 + ls2 - ls1, s1) == 0;
	}
	static bool is_tag(const char_type* str) {
		return std::regex_match (str, IS_TAGW);
	}
	template<typename _Found_proc>
	static void find_attr(const std::wstring& str, _Found_proc& found_proc) {
		auto words_begin =
		      std::wsregex_iterator(str.begin(), str.end(), ATTRSW);
		    auto words_end = std::wsregex_iterator();

		for (std::wsregex_iterator i = words_begin; i != words_end; ++i) {
		    std::wsmatch match = *i;
		    found_proc(match[1], match[2], match[3]);
		}
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
	size_type capacity() const {
		return _M_end - _M_begin;
	}
	size_type length() const {
		return _M_cur - _M_begin;
	}
	string_builder& length(size_type _new_length) {
		if (_new_length < length()) {
			_M_cur = _M_begin + _new_length;
			*_M_cur = 0;
		}
		return *this;
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
	char_type* begin() const {
		return _M_begin;
	}
	char_type* end() const {
		return _M_cur;
	}
	const char_type* cbegin() const {
		return _M_begin;
	}
	const char_type* cend() const {
		return _M_cur;
	}
	bool equals(const char_type* str) const {
		return xml_defs<char_type>::strcmp(_M_begin, str);
	}
	bool endswith(const char_type* str) const {
		size_t ls2 = xml_defs<char_type>::strlen(str);
		if (length() < ls2) return false;
		return xml_defs<char_type>::strcmp(_M_cur - ls2, str);
	}
};

template<typename _CharT>
struct attributes {
	std::vector<attribute<_CharT> > attrs;
	void operator()(const std::basic_string<_CharT>& ns,
			const std::basic_string<_CharT>& name,
			const std::basic_string<_CharT>& value) {
		attribute<_CharT> attr;
		attr.prefix = ns;
		attr.name = name;
		attr.value = value;
		attrs.push_back(attr);
	}
};

template<typename _CharT>
std::vector<attribute<_CharT> > parse_attrs(const _CharT* str) {
	attributes<_CharT> attrs;
	xml_defs<_CharT>::find_attr(str,attrs);
	return attrs.attrs;
}

template<typename _CharT>
bool parse_tag(const _CharT* str,
		tag_handler<_CharT>* _handler) {

	return true;
}

template<typename _CharT>
bool parse_doctype(const _CharT* str,
		tag_handler<_CharT>* _handler) {

	return true;
}

template<typename _CharT>
bool parse_xml_decl(const _CharT* str,
		tag_handler<_CharT>* _handler) {
	std::vector<attribute<_CharT> > attrs = parse_attrs(str);
	const _CharT* version = NULL;
	const _CharT* encoding = NULL;
	const _CharT* standalone = NULL;
	typename std::vector<attribute<_CharT> >::iterator it = attrs.begin();
	typename std::vector<attribute<_CharT> >::iterator end = attrs.end();
	for (; it != end; ++it) {
		if (it->name == xml_defs<_CharT>::text(XC_VERSION))
			version = it->value.c_str();
		if (it->name == xml_defs<_CharT>::text(XC_ENCODING))
			encoding = it->value.c_str();
		if (it->name == xml_defs<_CharT>::text(XC_STANDALONE))
			standalone = it->value.c_str();
	}
	_handler->xml_decl(version,encoding,standalone);
	return true;
}

template<typename _CharT>
bool read_tag(string_builder<_CharT,std::allocator<_CharT>> &builder,
		std::basic_istream<_CharT>& is,
		tag_handler<_CharT>* _handler) {
	bool in_tag = false;
	bool in_comment = false;
	bool in_cdata = false;
	bool in_doctype = false;
	bool in_xml = false;
	int tags_count = 0;
	while(is) {
		_CharT ch = (_CharT) is.get();
		builder.push(ch);
		if (in_tag) {
			if (ch == *(xml_defs<_CharT>::text(XC_GT))) {
				bool result = parse_tag(builder.c_str(), _handler);
				builder.clear();
				return result;
			}
		} else if (in_doctype) {
			if (ch == *(xml_defs<_CharT>::text(XC_LT)))
				tags_count++;
			if (ch == *(xml_defs<_CharT>::text(XC_GT))) {
				if (tags_count == 0) {
					bool result = parse_doctype(builder.c_str(), _handler);
					builder.clear();
					return result;
				} else
					tags_count--;
			}
		} else if (in_comment) {
			const _CharT* begin_comments = xml_defs<_CharT>::text(XC_BEGIN_COMMENT);
			const _CharT* end_comments = xml_defs<_CharT>::text(XC_END_COMMENT);
			if (builder.endswith(end_comments)) {
				builder.length(builder.length() - xml_defs<_CharT>::strlen(end_comments));
				_handler->comments(builder.c_str() + xml_defs<_CharT>::strlen(begin_comments),
						builder.cend());
				return true;
			}
		} else if (in_xml) {
			const _CharT* begin_xml_decl = xml_defs<_CharT>::text(XC_BEGIN_XML_DECL);
			const _CharT* end_xml_decl = xml_defs<_CharT>::text(XC_END_XML_DECL);
			if (builder.endswith(end_xml_decl)) {
				builder.length(builder.length() - xml_defs<_CharT>::strlen(end_xml_decl));
				bool result = parse_xml_decl(builder.c_str() + xml_defs<_CharT>::strlen(begin_xml_decl), _handler);
				builder.clear();
				return result;
			}
		} else if (in_cdata) {
			const _CharT* begin_cdata = xml_defs<_CharT>::text(XC_BEGIN_CDATA);
			const _CharT* end_cdata = xml_defs<_CharT>::text(XC_END_CDATA);
			if (builder.endswith(end_cdata)) {
				builder.length(builder.length() - xml_defs<_CharT>::strlen(end_cdata));
				_handler->characters(builder.c_str() + xml_defs<_CharT>::strlen(begin_cdata),
						builder.cend());
				return true;
			}
		} else {
			if (builder.endswith(xml_defs<_CharT>::text(XC_BEGIN_DOCTYPE)))
				in_doctype = true;
			else if (builder.endswith(xml_defs<_CharT>::text(XC_BEGIN_CDATA)))
				in_cdata = true;
			else if (builder.endswith(xml_defs<_CharT>::text(XC_BEGIN_COMMENT)))
				in_comment = true;
			else if (builder.endswith(xml_defs<_CharT>::text(XC_BEGIN_XML_DECL)))
				in_xml = true;
			else if (xml_defs<_CharT>::is_tag(builder.c_str()))
				in_tag = true;
		}
	}
	_handler->fatal_error(xml_parser_exception("Unexpected end of file."));
	return false;
}

template<typename _CharT>
void parse_xml(std::basic_istream<_CharT>& is, tag_handler<_CharT>* _handler) {
	string_builder<_CharT,std::allocator<_CharT>> builder;
	while(is) {
		_CharT ch = (_CharT) is.get();
		if (ch == *(xml_defs<_CharT>::text(XC_LT))) {
			if (builder.length() != 0)
				_handler->characters(builder.begin(), builder.cend());
			builder.clear();
			builder.push(ch);
			if (!read_tag(builder, is, _handler))
				break;
		} else
			builder.push(ch);
	}
}

void basic_xml_parser<char>::parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {
	parse_xml(_input_stream, _handler);
}

void basic_xml_parser<wchar_t>::parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {
	parse_xml(_input_stream, _handler);
}
