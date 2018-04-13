/*
 * logger.hpp
 *
 *  Created on: 19/04/2011
 *      Author: Alan
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <utils/utils_defs.hpp>
#include <string>
#include <utils/log/appender.hpp>

// define the log level
#ifndef LOG_LEVEL
#define LOG_LEVEL 2
#endif

#define LL_TRACE 0
#define LL_DEBUG 1
#define LL_INFO  2
#define LL_WARN  3
#define LL_ERROR 4
#define LL_FATAL 5
#define LL_NO_LOG 6

namespace utils {

namespace log {

class logger {
protected:
	std::string _M_logger_name;
	appender* _M_appender;
	friend class logger_factory;
	logger(const std::string& logger_name, appender* _appender): _M_logger_name(logger_name), _M_appender(_appender) {
	}
public:

	logger(const logger& other): _M_logger_name(other._M_logger_name), _M_appender(other._M_appender) {
	}

	logger & operator = (const logger &other) {
		_M_logger_name = other._M_logger_name;
		_M_appender = other._M_appender;
		return *this;
	}

	virtual ~logger() { }

	virtual void trace(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_TRACE, c_str);
	}
	virtual void trace(const char* _file,  int _line, const char* _method, const std::string& str) {
		trace(_file, _line, _method, str.c_str());
	}

	virtual void trace(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		trace(_file, _line, _method, exc.what());
	}

	virtual void trace(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		trace(_file, _line, _method, msg.c_str());
	}

	virtual void debug(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_DEBUG, c_str);
	}
	virtual void debug(const char* _file,  int _line, const char* _method, const std::string& str) {
		debug(_file, _line, _method, str.c_str());
	}

	virtual void debug(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		debug(_file, _line, _method, exc.what());
	}

	virtual void debug(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		debug(_file, _line, _method, msg.c_str());
	}

	virtual void info(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_INFO, c_str);
	}
	virtual void info(const char* _file,  int _line, const char* _method, const std::string& str) {
		info(_file, _line, _method, str.c_str());
	}

	virtual void info(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		info(_file, _line, _method, exc.what());
	}

	virtual void info(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		info(_file, _line, _method, msg.c_str());
	}

	virtual void warn(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_WARN, c_str);
	}
	virtual void warn(const char* _file,  int _line, const char* _method, const std::string& str) {
		warn(_file, _line, _method, str.c_str());
	}

	virtual void warn(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		warn(_file, _line, _method, exc.what());
	}

	virtual void warn(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		warn(_file, _line, _method, msg.c_str());
	}

	virtual void error(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_ERROR, c_str);
	}
	virtual void error(const char* _file,  int _line, const char* _method, const std::string& str) {
		error(_file, _line, _method, str.c_str());
	}

	virtual void error(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		error(_file, _line, _method, exc.what());
	}

	virtual void error(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		error(_file, _line, _method, msg.c_str());
	}

	virtual void fatal(const char* _file, int _line, const char* _method, const char* c_str) {
		_M_appender->append(_file, _line, _method,_M_logger_name.c_str(),LL_FATAL, c_str);
	}
	virtual void fatal(const char* _file,  int _line, const char* _method, const std::string& str) {
		fatal(_file, _line, _method, str.c_str());
	}

	virtual void fatal(const char* _file,  int _line, const char* _method, const std::exception& exc) {
		fatal(_file, _line, _method, exc.what());
	}

	virtual void fatal(const char* _file,  int _line, const char* _method, const std::exception& exc, const std::string& str) {
		std::string msg = str;
		msg += ": ";
		msg += exc.what();
		fatal(_file, _line, _method, msg.c_str());
	}


};

#define CREATE_LOGGER(c) static utils::log::logger& _logger() { \
	static utils::log::logger _logger_instance = utils::log::logger_factory::get_logger(c); \
	return _logger_instance; \
}

#define LOG_TRACE(...) if (LOG_LEVEL <= LL_TRACE) _logger().trace(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define LOG_DEBUG(...) if (LOG_LEVEL <= LL_DEBUG) _logger().debug(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define LOG_INFO(...)  if (LOG_LEVEL <= LL_INFO ) _logger().info (__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define LOG_WARN(...)  if (LOG_LEVEL <= LL_WARN ) _logger().warn (__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define LOG_ERROR(...) if (LOG_LEVEL <= LL_ERROR) _logger().error(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define LOG_FATAL(...) if (LOG_LEVEL <= LL_FATAL) _logger().fatal(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

}  // namespace log

}  // namespace utils

#endif /* LOGGER_HPP_ */
