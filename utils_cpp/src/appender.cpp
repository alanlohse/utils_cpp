/*
 * appender.cpp
 *
 *  Created on: 19/04/2011
 *      Author: Alan
 */

#include <utils/log/appender.hpp>
#include <utils/log/logger.hpp>

namespace utils {

namespace log {

void console_appender::append(int level, char ch) {
	if (level >= LL_ERROR)
		std::cerr << ch;
	else
		std::cout << ch;
}

void console_appender::append(int level, const char* c_str) {
	if (level >= LL_ERROR)
		std::cerr << c_str;
	else
		std::cout << c_str;
}

void console_appender::append(int level, int i) {
	if (level >= LL_ERROR)
		std::cerr << i;
	else
		std::cout << i;
}

}

}

