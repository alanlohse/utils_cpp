/*
 * url.hpp
 *
 *  Created on: 13/04/2011
 *      Author: Alan
 */

#ifndef URL_HPP_
#define URL_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>
#include <string>
#include <cstring>

#ifdef UTILS_LINUX
char *strlwr(char *str);
#endif

namespace utils {

namespace net {

class malformed_url_exception : public utils_exception {
public:
	malformed_url_exception() throw() :
		utils_exception()
	{
	}
	malformed_url_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

class url {
protected:
	std::string host;
	std::string path;
	std::string query;
	std::string protocol;
	int port;

public:
	url(const std::string& str) {
		bool host_readed = false;
		bool path_readed = false;
		bool proto_readed = false;
//		bool port_readed = false;
		port = -1;
		std::string partial;
		std::string::const_iterator it = str.begin();
		for (; it != str.end(); ++it) {
			char ch = *it;
			if (ch == ':') {
				if (*(++it) == '/') {
					if (*(++it) == '/') {
						if (proto_readed)throw malformed_url_exception("Protocol defined already.");
						protocol = partial;
						partial = std::string();
						proto_readed = true;
					} else throw malformed_url_exception("expected ://");
				} else if (*it >= '0' && *it <= '9' && !host_readed) {
					host = partial;
					partial = std::string();
					host_readed = true;
					for (; it != str.end() && *it != '/'; ++it) {
						partial += *it;
					}
					char * ptr = NULL;
					port = strtol(partial.c_str(),&ptr,10);
					if (*ptr != 0) throw malformed_url_exception("Invalid port number.");
					--it;
					partial = std::string();
//					port_readed = true;
				} else throw malformed_url_exception("Invalid URL.");
			} else if (ch == '/' && !host_readed) {
				host = partial;
				partial = std::string("/");
				host_readed = true;
				port = -1;
//				port_readed = true;
			} else if (ch == '?') {
				if (path_readed) throw malformed_url_exception("Invalid URL.");
				path = partial;
				partial = std::string();
				path_readed = true;
			} else
				partial += ch;
		}
		strlwr(&(*host.begin()));
		strlwr(&(*protocol.begin()));
		if (path_readed)
			query = partial;
		else
			path = partial;
		if (port == -1) {
			if (protocol == "http")
				port = 80;
			else if (protocol == "file")
				port = 0;
			else if (protocol == "https")
				port = 443;
			else if (protocol == "pop3")
				port = 110;
			else if (protocol == "smtp")
				port = 25;
			else if (protocol == "ftp")
				port = 21;
			else if (protocol == "pop3s")
				port = 995;
			else if (protocol == "smtps")
				port = 465;
			else if (protocol == "sftp")
				port = 115;
			else if (protocol == "pop2")
				port = 109;
		}
	}

	const std::string& get_host() {
		return host;
	}
	const std::string& get_path() {
		return path;
	}
	const std::string& get_query() {
		return query;
	}
	const std::string& get_protocol() {
		return protocol;
	}
	int get_port() const {
		return port;
	}
};

}  // namespace net

}  // namespace utils

#endif /* URL_HPP_ */
