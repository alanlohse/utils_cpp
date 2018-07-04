/*
 * xml_binding.hpp
 *
 *  Created on: 2 de jul de 2018
 *      Author: alan.lohse
 */

#ifndef UTILS_XML_XML_BINDING_HPP_
#define UTILS_XML_XML_BINDING_HPP_

#include <utils/xml/xml_parser.hpp>
#include <utils/functions/bound_funcs.hpp>
#include <utils/conversion.hpp>
#include <vector>
#include <string>

namespace utils {

namespace xml {

template<typename _CharT, class _String_alloc = std::allocator<_CharT> >
struct xml_binding_attribute {
	typedef _CharT char_type;
	typedef typename _String_alloc::template rebind<char_type>::other string_allocator_type;
	typedef std::basic_string<char_type, string_allocator_type> string_type;
	const char_type* attribute_name;
	const char_type* namespace_name;

	xml_binding_attribute(const char_type* _attribute_name, const char_type* _namespace_name) :
			attribute_name(_attribute_name), namespace_name(_namespace_name) {
	}

	virtual ~xml_binding_attribute() {
	}

	virtual string_type get_value_as_string() const = 0;

	virtual void set_value(const string_type& _str_val) = 0;
};

}

}

#endif /* UTILS_XML_XML_BINDING_HPP_ */
