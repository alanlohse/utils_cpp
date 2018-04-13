/*
 * pattern.cpp
 *
 *  Created on: 22/04/2011
 *      Author: Alan
 */

#include <utils/log/appender.hpp>
#include <utils/thread/thread.hpp>
#include <ctime>

namespace utils {

namespace log {

const char* _levels[] = {"TRACE","DEBUG","INFO","WARN","ERROR","FATAL"};


void pattern_layout::do_layout(appender* appnd, const char* _file,  int _line, const char* _method, const char* category, int level, const char* msg) {

	const char* fmt = _M_pattern.c_str();
	for (; *fmt != 0; fmt++) {
		if (*fmt == '%') {
			switch (*(++fmt)) {
			case '%':
				appnd->append(level,'%');
				break;
			case 'm':
				appnd->append(level,msg);
				break;
			case 'n':
				appnd->append(level,"\n");
				break;
			case 'f':
				appnd->append(level,_file);
				break;
			case 'l':
				appnd->append(level,_line);
				break;
			case 'F':
				appnd->append(level,_method);
				break;
			case 'c':
				appnd->append(level,category);
				break;
			case 'p':
				if (level >= 0 && level < 6)
					appnd->append(level,_levels[level]);
				break;
			case 'd': {
				time_t rawtime;
				tm * timeinfo;
				char buf[80];
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				strftime(buf, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
				appnd->append(level,buf);
			} break;
			case 't': {
				int tid = thread::get_current_thread_id();
				char nm[16];
				ltoa(tid,nm,10);
				appnd->append(level,nm);
			} break;
			}
		} else {
			appnd->append(level,*fmt);
		}
	}

}

}

}
