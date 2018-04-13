/*
 * layout.hpp
 *
 *  Created on: 21/04/2011
 *      Author: Alan
 */

#ifndef LAYOUT_HPP_
#define LAYOUT_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>
#include <utils/funcs.hpp>
#include <iostream>
#include <string>

namespace utils {

namespace log {

class appender;

class layout {
public:
	virtual ~layout() { }

	virtual void do_layout(appender* appnd, const char* _file,  int _line, const char* _method, const char* category, int level, const char* msg) = 0;
};

class pattern_layout : public layout {
protected:
	std::string _M_pattern;
public:
	pattern_layout(const std::string& pattern): _M_pattern(pattern) {

	}

	virtual ~pattern_layout() { }

	virtual void do_layout(appender* appnd, const char* _file,  int _line, const char* _method, const char* category, int level, const char* msg);
};


}

}

#endif /* LAYOUT_HPP_ */
