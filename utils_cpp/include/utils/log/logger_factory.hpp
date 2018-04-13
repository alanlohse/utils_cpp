/*
 * logger_factory.hpp
 *
 *  Created on: 22/04/2011
 *      Author: Alan
 */

#ifndef LOGGER_FACTORY_HPP_
#define LOGGER_FACTORY_HPP_

#include <utils/log/appender.hpp>
#include <utils/log/layout.hpp>
#include <utils/log/logger.hpp>
#include <string>
#include <vector>

namespace utils {

namespace log {

class logger_factory {
protected:
	logger_factory();

	static logger_factory& get_instance() {
		static logger_factory _instance;
		return _instance;
	}

	typedef std::vector<appender*, DEFAULT_ALLOCATOR<appender*> > appender_list_type;
	typedef std::vector<layout*, DEFAULT_ALLOCATOR<layout*> > layout_list_type;
	appender_list_type _M_appenders;
	layout_list_type _M_layouts;

public:

	virtual ~logger_factory();

	static logger get_logger(const std::string& name) {
		return logger_factory::get_instance().create_logger(name);
	}

	logger create_logger(const std::string& name);

};

}

}

#endif /* LOGGER_FACTORY_HPP_ */
