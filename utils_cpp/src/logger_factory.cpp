/*
 * logger_factory.cpp
 *
 *  Created on: 22/04/2011
 *      Author: Alan
 */

#include <utils/log/logger_factory.hpp>

namespace utils {

namespace log {

logger_factory::logger_factory(): _M_appenders(), _M_layouts() {
	_M_layouts.push_back(new pattern_layout("%d %f %p [%t] (%F) - %m%n"));
	_M_appenders.push_back(new console_appender(_M_layouts[0]));
}

logger_factory::~logger_factory() {
	appender_list_type::iterator it1 = _M_appenders.begin();
	for (; it1 != _M_appenders.begin(); it1++) {
		if (*it1 != NULL)
			delete (*it1);
	}
	layout_list_type::iterator it2 = _M_layouts.begin();
	for (; it2 != _M_layouts.begin(); it2++) {
		if (*it2 != NULL)
			delete (*it2);
	}
}

logger logger_factory::create_logger(const std::string& name) {
	return logger(name,_M_appenders[0]);
}


}  // namespace log

}  // namespace utils
