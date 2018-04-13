/*
 * request_ostream.hpp
 *
 *  Created on: 29/04/2011
 *      Author: Alan
 */

#ifndef REQUEST_OSTREAM_HPP_
#define REQUEST_OSTREAM_HPP_

#include <utils/utils_defs.hpp>
#include <string>
#include <iostream>

namespace utils {

namespace io {

class limited_ostream : public std::ostream {
protected:
	std::streamsize _M_write_limit;
	std::streamsize _M_wrote;
public:
	limited_ostream(): std::ostream(0), _M_write_limit(0), _M_wrote(0) { }
	limited_ostream(const std::ostream& out, std::streamsize write_limit): std::ostream(out.rdbuf()), _M_write_limit(write_limit), _M_wrote(0) { }

	limited_ostream& operator = (const limited_ostream &rhs) {
		this->init(rhs.rdbuf());
		_M_write_limit = rhs._M_write_limit;
		_M_wrote = rhs._M_wrote;
		return *this;
	}

	limited_ostream& put ( char c ) {
		if (_M_wrote < _M_write_limit)
			std::ostream::put(c);
		++_M_wrote;
		return *this;
	}

	limited_ostream& write ( const char* s , std::streamsize n ) {
		std::streamsize nl = _M_write_limit-_M_wrote;
		if (nl < n) {
			std::ostream::write(s,nl);
			_M_wrote += nl;
		} else {
			std::ostream::write(s,n);
			_M_wrote += n;
		}
		return *this;
	}

};

}

}

#endif /* REQUEST_OSTREAM_HPP_ */
