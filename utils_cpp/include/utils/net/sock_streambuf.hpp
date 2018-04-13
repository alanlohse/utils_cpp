/*
 * sock_streambuf.hpp
 *
 *  Created on: 14/04/2011
 *      Author: Alan
 */

#ifndef SOCK_STREAMBUF_HPP_
#define SOCK_STREAMBUF_HPP_

#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>
#include <string>
#include <iostream>

namespace utils {

namespace net {

class sock_streambuf : public std::basic_streambuf<char> {
public:

	sock_streambuf(int fd, int timeout):std::basic_streambuf<char>(), _M_allocator(), _M_fd(fd), _M_timeout(timeout), _M_refs(0) {
	}

	sock_streambuf(const sock_streambuf& other):std::basic_streambuf<char>(), _M_allocator(other._M_allocator), _M_fd(other._M_fd), _M_timeout(other._M_timeout), _M_refs(0) {
	}

	virtual ~sock_streambuf();

protected:

	std::streamsize
    	socket_read(char_type* __s, std::streamsize __n);
    std::streamsize
    	socket_write(const char_type* __s, std::streamsize __n);

    virtual std::streamsize
    	showmanyc();

    virtual int	underflow ();
    virtual int	overflow (int c = EOF);
    virtual int	doallocate ();
    virtual int	sync();

    int do_flush();

    friend class socket_iostream;
	DEFAULT_ALLOCATOR<char_type> _M_allocator;
	int _M_fd;
	int _M_timeout;
	int _M_refs;

	bool is_leak() {
		return _M_refs <= 0;
	}
	sock_streambuf* add_ref() {
		++_M_refs;
		return this;
	}
	int rem_ref() {
		return --_M_refs;
	}
};

}

}

#endif /* SOCK_STREAMBUF_HPP_ */
