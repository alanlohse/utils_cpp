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

template<typename _OwnerClass, typename _CharT, class _String_alloc = std::allocator<_CharT> >
struct xml_binding_attribute {
	typedef _CharT char_type;
	typedef _OwnerClass class_type;
	typedef typename _String_alloc::template rebind<char_type>::other string_allocator_type;
	typedef std::basic_string<char_type, string_allocator_type> string_type;
	const char_type* attribute_name;
	const char_type* namespace_name;

	xml_binding_attribute(const char_type* _attribute_name, const char_type* _namespace_name) :
			attribute_name(_attribute_name), namespace_name(_namespace_name) {
	}

	virtual ~xml_binding_attribute() {
	}

	virtual string_type get_value_as_string(const class_type* value) const = 0;

	virtual void set_value(class_type* value, const string_type& _str_val) = 0;
};

template<typename _InputType, typename _OwnerClass, typename _CharT, class _String_alloc = std::allocator<_CharT> >
struct xml_binding_attribute_field : public xml_binding_attribute<_OwnerClass,_CharT,_String_alloc> {
	typedef _CharT char_type;
	typedef _OwnerClass class_type;
	typedef typename _String_alloc::template rebind<char_type>::other string_allocator_type;
	typedef std::basic_string<char_type, string_allocator_type> string_type;

	_InputType _OwnerClass::*field;

	xml_binding_attribute_field(const char_type* _attribute_name, const char_type* _namespace_name, _InputType _OwnerClass::*_field) :
		xml_binding_attribute<_OwnerClass,_CharT,_String_alloc>(_attribute_name, _namespace_name), field(_field) {
	}

	virtual ~xml_binding_attribute_field() {
	}

	virtual string_type get_value_as_string(const class_type* value) const {
		return utils::to_string<int>(value->*field);
	}

	virtual void set_value(class_type* value, const string_type& _str_val) {
		value->*field = utils::from_string<_InputType>(_str_val);
	}
};

template<typename _InputType, typename _OwnerClass, typename _CharT, class _String_alloc = std::allocator<_CharT> >
struct xml_binding_attribute_function : public xml_binding_attribute<_OwnerClass,_CharT,_String_alloc> {
	typedef _CharT char_type;
	typedef _OwnerClass class_type;
	typedef typename _String_alloc::template rebind<char_type>::other string_allocator_type;
	typedef std::basic_string<char_type, string_allocator_type> string_type;

	typedef _InputType (_OwnerClass::*getter_type)() const;
	getter_type getter;

	typedef void (_OwnerClass::*setter_type)(const _InputType& val);
	setter_type setter;

	xml_binding_attribute_function(const char_type* _attribute_name, const char_type* _namespace_name, getter_type _getter, setter_type _setter) :
		xml_binding_attribute<_OwnerClass,_CharT,_String_alloc>(_attribute_name, _namespace_name), getter(_getter), setter(_setter) {
	}

	virtual ~xml_binding_attribute_function() {
	}

	virtual string_type get_value_as_string(const class_type* value) const {
		return utils::to_string(value->*getter());
	}

	virtual void set_value(class_type* value, const string_type& _str_val) {
		value->*setter(utils::from_string<_InputType>(_str_val));
	}
};

}

}

#endif /* UTILS_XML_XML_BINDING_HPP_ */
