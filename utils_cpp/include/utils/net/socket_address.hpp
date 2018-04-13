/*
 * socket_address.hpp
 *
 *  Created on: 24/04/2011
 *      Author: Alan
 */

#ifndef SOCKET_ADDRESS_HPP_
#define SOCKET_ADDRESS_HPP_

#include <utils/utils_defs.hpp>
#include <cstring>

namespace utils {

namespace net {

namespace __socket_impl {
class socket_base;
}  // namespace __socket_impl

class socket_address {
protected:
	friend class __socket_impl::socket_base;
#define ADDRSIZE 32
	char buff[ADDRSIZE];
	socket_address() {
		::memset(buff,0,ADDRSIZE);
	}
public:
	socket_address(const std::string& hostname, u_short port);
	~socket_address() {
	}

	std::string get_host_name() const;

	int get_length() const;

	const void* get_sockaddr() const;

	std::string get_ip_address() const;

	u_short get_port() const;
};

}

}

#endif /* SOCKET_ADDRESS_HPP_ */
