/*
 * geometry_utl.hpp
 *
 *  Created on: 18/08/2014
 *      Author: Alan
 */

#ifndef GEOMETRY_UTL_HPP_
#define GEOMETRY_UTL_HPP_

#include <utils/math/geometry/vectors.hpp>

namespace utils {

namespace math {

namespace geometry_utl {

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_lines(const vector<_Float_a,_Vec_size>& o1, const vector<_Float_a,_Vec_size>& d1,
		const vector<_Float_b,_Vec_size>& o2, const vector<_Float_b,_Vec_size>& d2) {
	if (o1 == o2)  return o1;
	vector<_Float_b,_Vec_size> dc = o2 - o1;
	if (dc * (d1 & d2) == 0.0) {
		vector<_Float_a,_Vec_size> dacdb = d1 & d2;
		_Float_b s = ((dc & d2) * dacdb) / dacdb.sqr();
	    return o1 + (d1 * s);
	}
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_line_segments(const vector<_Float_a,_Vec_size>& a1, const vector<_Float_a,_Vec_size>& a2,
		const vector<_Float_b,_Vec_size>& b1, const vector<_Float_b,_Vec_size>& b2) {
	if (a1 == b1 || a1 == b2)  return a1;
	if (a2 == b1 || a2 == b2)  return a2;
	vector<_Float_a,_Vec_size> da = a2 - a1;
	vector<_Float_b,_Vec_size> db = b2 - b1;
	vector<_Float_b,_Vec_size> dc = b1 - a1;
	if (dc * (da & db) == 0.0) {
		vector<_Float_a,_Vec_size> dacdb = da & db;
		_Float_b s = ((dc & db) * dacdb) / dacdb.sqr();
		if (s >= 0 && s <= 1)
			return a1 + (da * s);
	}
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_line_plane(const vector<_Float_a,_Vec_size>& lo, const vector<_Float_a,_Vec_size>& ld,
		const vector<_Float_b,_Vec_size>& po, const vector<_Float_b,_Vec_size>& pn) {
	_Float_b r = (pn * (po - lo))/(pn * ld);
	if (!isinf(r) && !isnan(r))
		return lo + (ld * r);
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_segment_plane(const vector<_Float_a,_Vec_size>& a1, const vector<_Float_a,_Vec_size>& a2,
		const vector<_Float_b,_Vec_size>& po, const vector<_Float_b,_Vec_size>& pn) {
	vector<_Float_a,_Vec_size> ld = a2 - a1;
	_Float_b r = (pn * (po - a1))/(pn * ld);
	if (!isinf(r) && !isnan(r) && r >= 0 && r <= 1)
		return a1 + (ld * r);
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_line_triangle(const vector<_Float_a,_Vec_size>& lo, const vector<_Float_a,_Vec_size>& ld,
		const vector<_Float_b,_Vec_size>& t1, const vector<_Float_b,_Vec_size>& t2, const vector<_Float_b,_Vec_size>& t3) {
	vector<_Float_b,_Vec_size> u = t2 - t1;
	vector<_Float_b,_Vec_size> v = t3 - t1;
	vector<_Float_b,_Vec_size> n = u & v;
	vector<_Float_a,_Vec_size> w0 = lo - t1;
	if (n.sqr() > 0) {
		_Float_b a = - n * w0;
		_Float_b b = n * ld;
		if (b != 0) {
			_Float_b l = a / b;
			vector<_Float_a,_Vec_size> ip = lo + (ld * l);
			_Float_b uu, uv, vv, wu, wv, det;
			uu = u * u;
			uv = u * v;
			vv = v * v;
			vector<_Float_a,_Vec_size> w = ip - t1;
			wu = w * u;
			wv = w * v;
			det = uv * uv - uu * vv;

			_Float_b s, t;
			s = (uv * wv - vv * wu) / det;
			if (s < 0.0 || s > 1.0)
				return vector<_Float_a,_Vec_size>::NaN_vector();
			t = (uv * wu - uu * wv) / det;
			if (t < 0.0 || (s + t) > 1.0)
				return vector<_Float_a,_Vec_size>::NaN_vector();
			return ip;
		}
	}
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

template <typename _Float_a, typename _Float_b, size_t _Vec_size>
vector<_Float_a,_Vec_size> intersect_segment_triangle(const vector<_Float_a,_Vec_size>& a1, const vector<_Float_a,_Vec_size>& a2,
		const vector<_Float_b,_Vec_size>& t1, const vector<_Float_b,_Vec_size>& t2, const vector<_Float_b,_Vec_size>& t3) {
	vector<_Float_a,_Vec_size> ld = a2 - a1;
	vector<_Float_a,_Vec_size> ip = intersect_line_triangle(a1,ld,t1,t2,t3);
	_Float_a s = ((ip - a1)*ld) / ld.sqr();
	if (s >= 0 && s <= 1)
		return ip;
	return vector<_Float_a,_Vec_size>::NaN_vector();
}

} // geometry_utl

} // math

} // utils

#endif /* GEOMETRY_UTL_HPP_ */
