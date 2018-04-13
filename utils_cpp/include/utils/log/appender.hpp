/*
 * appender.hpp
 *
 *  Created on: 19/04/2011
 *      Author: Alan
 */

#ifndef APPENDER_HPP_
#define APPENDER_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>
#include <utils/funcs.hpp>
#include <utils/log/layout.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace utils {

namespace log {

class appender {
protected:
	layout* _M_layout;
	typedef std::vector<std::string, DEFAULT_ALLOCATOR<std::string> > filter_list_type;
	filter_list_type _M_filters;
public:

	appender(layout* _layout): _M_layout(_layout), _M_filters() {
	}

	virtual ~appender() { }

	virtual void set_layout(layout* _layout) {
		_M_layout = _layout;
	}

	virtual layout* get_layout() {
		return _M_layout;
	}

	virtual void append(const char* _file, int _line, const char* _method, const char* category, int level, const char* msg) {
		if (category && !_M_filters.empty()) {
			filter_list_type::iterator it = _M_filters.begin();
			for (; it != _M_filters.end(); it++) {
				if (strstr(category,(*it).c_str())) {
					_M_layout->do_layout(this,_file,_line,_method, category, level, msg);
					return;
				}
			}
		} else {
			_M_layout->do_layout(this,_file,_line,_method, category, level, msg);
		}
	}

	virtual void append(int level, char ch) = 0;

	virtual void append(int level, const char* c_str) = 0;

	virtual void append(int level, int i) = 0;

	virtual void add_filter(const char* filter) {
		_M_filters.push_back(filter);
	}

};

class console_appender : public appender {
public:

	console_appender(layout* _layout): appender(_layout) {
	}

	virtual ~console_appender() { }

	virtual void append(int level, char ch);

	virtual void append(int level, const char* c_str);

	virtual void append(int level, int i);

};


}

}

#endif /* APPENDER_HPP_ */
