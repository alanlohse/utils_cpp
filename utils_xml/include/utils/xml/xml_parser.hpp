/*
 * xml_parser.hpp
 *
 *  Created on: 14 de jun de 2018
 *      Author: alan.lohse
 */

#ifndef UTILS_XML_XML_PARSER_HPP_
#define UTILS_XML_XML_PARSER_HPP_

#include <utils/config.hpp>
#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>
#include <vector>
#include <istream>
#include <type_traits>

namespace utils {

namespace xml {

class xml_parser_exception : public utils_exception {
public:
	xml_parser_exception() throw() :
		utils_exception()
	{
	}
	xml_parser_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

template<typename _CharT>
struct attribute {
	const _CharT* name;
	const _CharT* uri;
	const _CharT* value;
};

template<typename _CharT>
class tag_handler {
public:

	typedef _CharT char_type;

	virtual ~tag_handler () = default;

	virtual void start_document() = 0;

	virtual void end_document() = 0;

	virtual void start_element(const char_type* tag, const char_type* uri, const std::vector<attribute<char_type> >& attributes) = 0;

	virtual void end_element(const char_type* tag, const char_type* uri) = 0;

	virtual std::basic_streambuf<char_type, std::char_traits<char_type> >* resolve_entity (const char_type* public_id, const char_type* system_id) = 0;

	virtual void xml_decl(const char_type* version, const char_type* encoding, const char_type* standalone) = 0;

	virtual void start_prefix_mapping (const char_type* prefix, const char_type* uri) = 0;

	virtual void end_prefix_mapping (const char_type* prefix) = 0;

	virtual void characters (const char_type* begin, const char_type* end) = 0;

	virtual void comments (const char_type* begin, const char_type* end) = 0;

	virtual void ignorable_whitespace (const char_type* ch, int start, int length) = 0;

	virtual void skipped_entity (const char_type* name) = 0;

	virtual void warning (const xml_parser_exception& e) = 0;

	virtual void error (const xml_parser_exception& e) = 0;

	virtual void fatal_error (const xml_parser_exception& e) = 0;

};


template<typename _CharT>
class basic_xml_parser {
public:

	typedef _CharT char_type;

	basic_xml_parser() {
	}

	~basic_xml_parser() { }

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>& _handler) {
		parse(_input_stream, &_handler);
	}

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler) {
	}

};

typedef basic_xml_parser<char> xml_parser;
typedef basic_xml_parser<wchar_t> wxml_parser;

template<>
class basic_xml_parser<char> {
public:

	typedef char char_type;

	basic_xml_parser() {
	}

	~basic_xml_parser() { }

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>& _handler) {
		parse(_input_stream, &_handler);
	}

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler);

};

template<>
class basic_xml_parser<wchar_t> {
public:

	typedef wchar_t char_type;

	basic_xml_parser() {
	}

	~basic_xml_parser() { }

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>& _handler) {
		parse(_input_stream, &_handler);
	}

	void parse(std::basic_istream<char_type>& _input_stream, tag_handler<char_type>* _handler);

};


} // namespace xml

} // namespace utils

#endif /* UTILS_XML_XML_PARSER_HPP_ */
