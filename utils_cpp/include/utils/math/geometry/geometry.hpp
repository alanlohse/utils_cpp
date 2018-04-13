/*
 * geometry.hpp
 *
 *  Created on: 16/08/2014
 *      Author: Alan
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <utils/math/geometry/vectors.hpp>
#include <utils/math/geometry/geometry_utl.hpp>

namespace utils {

namespace math {

template<typename _Float, size_t _Vec_size>
class geometric {
public:
	typedef _Float float_type;
	typedef vector<_Float, _Vec_size> vector_type;
	typedef geometric<_Float, _Vec_size> geometric_type;
public:
	geometric() { }
	virtual ~geometric() { }

	virtual vector_type intersect(const geometric_type& other) const = 0;
};



template<typename _Float, size_t _Vec_size>
class point : public geometric<_Float, _Vec_size> {
public:
	typedef typename geometric<_Float, _Vec_size>::float_type float_type;
	typedef typename geometric<_Float, _Vec_size>::vector_type vector_type;
	typedef typename geometric<_Float, _Vec_size>::geometric_type geometric_type;
public:
	vector_type origin;

	point() : origin() { }

	template<typename _Float_b>
	point(const vector<_Float_b, _Vec_size>& p) : origin(p) { }

	template<typename _Float_b>
	point(const point<_Float_b, _Vec_size>& p) : origin(p.origin) { }

	virtual ~point() {
	}

	virtual vector_type intersect(const geometric_type& other) const;

};



template<typename _Float, size_t _Vec_size>
class line : public geometric<_Float, _Vec_size> {
public:
	typedef typename geometric<_Float, _Vec_size>::float_type float_type;
	typedef typename geometric<_Float, _Vec_size>::vector_type vector_type;
	typedef typename geometric<_Float, _Vec_size>::geometric_type geometric_type;
public:
	vector_type origin;
	vector_type direction;
	line() : origin(), direction() {
	}
	template<typename _Float_b>
	line(const vector<_Float_b, _Vec_size>& _origin, const vector<_Float_b, _Vec_size>& _direction) : origin(_origin), direction(_direction) {
	}
	template<typename _Float_b>
	line(const line<_Float_b,_Vec_size>& other) : origin(other.origin), direction(other.direction) {
	}
	virtual ~line() {
	}

	virtual vector_type intersect(const geometric_type& other) const;

};


} // math

} // utils


#endif /* GEOMETRY_HPP_ */
