/*
 * matrix.hpp
 *
 *  Created on: 27/07/2014
 *      Author: Alan
 */

#ifndef MATRIX_UTL_HPP_
#define MATRIX_UTL_HPP_

#include <utils/math/math.hpp>
#include <utils/math/geometry/vectors.hpp>
#include <limits>

namespace utils {

namespace math {

namespace matrix_utl {

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type det_2x2(const float_type _m[_Rows][_Cols]) {
	float_type r = _m[0][0]*_m[1][1] - _m[1][0]*_m[0][1];
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type det_3x3(const float_type _m[_Rows][_Cols]) {
	float_type r = _m[0][0]*(_m[1][1]*_m[2][2] - _m[1][2]*_m[2][1])
          + _m[0][1]*(_m[1][2]*_m[2][0] - _m[1][0]*_m[2][2])
          + _m[0][2]*(_m[1][0]*_m[2][1] - _m[1][1]*_m[2][0]);
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type det_4x4(const float_type _m[_Rows][_Cols]) {
	float_type r = _m[0][3] * (  _m[1][0] * (_m[2][2] * _m[3][1] - _m[2][1] * _m[3][2])
						  + _m[1][1] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0])
						  + _m[1][2] * (_m[2][1] * _m[3][0] - _m[2][0] * _m[3][1]))
			+ _m[0][2] * (  _m[1][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1])
						  + _m[1][1] * (_m[2][3] * _m[3][0] - _m[2][0] * _m[3][3])
						  + _m[1][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]))
			+ _m[0][1] * (  _m[1][3] * (_m[2][2] * _m[3][0] - _m[2][0] * _m[3][2])
						  + _m[1][2] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0])
						  + _m[1][0] * (_m[2][3] * _m[3][2] - _m[2][2] * _m[3][3]))
			+ _m[0][0] * (  _m[1][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1])
						  + _m[1][2] * (_m[2][3] * _m[3][1] - _m[2][1] * _m[3][3])
						  + _m[1][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]));
	return r;
}

template <typename float_type>
inline void check_det(float_type det) {
	if (det == 0)
		throw math_exception("Matrix is not invertible.");
}

template <typename float_type, size_t _Rows, size_t _Cols>
float_type det_NxM(const float_type _m[_Rows][_Cols], size_t sz);

template <typename float_type, size_t _Rows, size_t _Cols>
struct adjugate_matrix {
	size_t rows;
	float_type _S_m[_Rows][_Cols];
	float_type determinant() {
		if (rows == 2)
			return det_2x2<float_type,_Rows,_Cols>(_S_m);
		if (rows == 3)
			return det_3x3<float_type,_Rows,_Cols>(_S_m);
		if (rows == 4)
			return det_4x4<float_type,_Rows,_Cols>(_S_m);
		return det_NxM<float_type,_Rows,_Cols>(_S_m,rows);
	}
};

template <typename float_type, size_t _Rows, size_t _Cols>
inline adjugate_matrix<float_type, _Rows, _Cols> make_adjugate_matrix(const float_type _m[_Rows][_Cols], size_t xrow, size_t xcol, size_t sz) {
	adjugate_matrix<float_type, _Rows, _Cols> r;
	r.rows = sz - 1;
	size_t ib = 0, jb = 0;
	for (size_t i = 0; i < sz; i++) {
		if (i != xrow) {
			jb = 0;
			for (size_t j = 0; j < sz; j++)
				if (j != xcol) {
					r._S_m[ib][jb] = _m[i][j];
					jb++;
				}
			ib++;
		}
	}
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type det_NxM(const float_type _m[_Rows][_Cols], size_t sz) {
	float_type r = 0;
	for (size_t j = 0; j < sz; j++) {
		r += (j%2==1 ? -1 : 1)*_m[0][j]*make_adjugate_matrix<float_type, _Rows, _Cols>(_m,0,j,sz).determinant();
	}
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline void invert_2x2(float_type _d[_Rows][_Cols], const float_type _s[_Rows][_Cols]) {
	float_type det = det_2x2<float_type, _Rows, _Cols>(_s);
	check_det(det);
	_d[0][0] = _s[1][1]/det;
	_d[0][1] = -_s[0][1]/det;
	_d[1][0] = -_s[1][0]/det;
	_d[1][1] = _s[0][0]/det;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline void invert_3x3(float_type _d[_Rows][_Cols], const float_type _s[_Rows][_Cols]) {
	float_type det = det_3x3<float_type, _Rows, _Cols>(_s);
	check_det(det);
	_d[0][0] = (_s[1][1]*_s[2][2] - _s[1][2]*_s[2][1])/det;
	_d[0][1] = (_s[0][2]*_s[2][1] - _s[0][1]*_s[2][2])/det;
	_d[0][2] = (_s[0][1]*_s[1][2] - _s[0][2]*_s[1][1])/det;
	_d[1][0] = (_s[1][2]*_s[2][0] - _s[1][0]*_s[2][2])/det;
	_d[1][1] = (_s[0][0]*_s[2][2] - _s[0][2]*_s[2][0])/det;
	_d[1][2] = (_s[0][2]*_s[1][0] - _s[0][0]*_s[1][2])/det;
	_d[2][0] = (_s[1][0]*_s[2][1] - _s[1][1]*_s[2][0])/det;
	_d[2][1] = (_s[0][1]*_s[2][0] - _s[0][0]*_s[2][1])/det;
	_d[2][2] = (_s[0][0]*_s[1][1] - _s[0][1]*_s[1][0])/det;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline void invert_4x4(float_type _d[_Rows][_Cols], const float_type _s[_Rows][_Cols]) {
	float_type det = det_4x4<float_type, _Rows, _Cols>(_s);
	check_det(det);
	_d[0][0] = (_s[1][1]*(_s[2][2]*_s[3][3] - _s[2][3]*_s[3][2]) + _s[1][2]*(_s[2][3]*_s[3][1] - _s[2][1]*_s[3][3]) + _s[1][3]*(_s[2][1]*_s[3][2] - _s[2][2]*_s[3][1]))/det;
	_d[0][1] = (_s[0][3]*_s[2][2]*_s[3][1] - _s[0][2]*_s[2][3]*_s[3][1] - _s[0][3]*_s[2][1]*_s[3][2] + _s[0][1]*_s[2][3]*_s[3][2] + _s[0][2]*_s[2][1]*_s[3][3] - _s[0][1]*_s[2][2]*_s[3][3])/det;
	_d[0][2] = (_s[0][2]*_s[1][3]*_s[3][1] - _s[0][3]*_s[1][2]*_s[3][1] + _s[0][3]*_s[1][1]*_s[3][2] - _s[0][1]*_s[1][3]*_s[3][2] - _s[0][2]*_s[1][1]*_s[3][3] + _s[0][1]*_s[1][2]*_s[3][3])/det;
	_d[0][3] = (_s[0][3]*_s[1][2]*_s[2][1] - _s[0][2]*_s[1][3]*_s[2][1] - _s[0][3]*_s[1][1]*_s[2][2] + _s[0][1]*_s[1][3]*_s[2][2] + _s[0][2]*_s[1][1]*_s[2][3] - _s[0][1]*_s[1][2]*_s[2][3])/det;
	_d[1][0] = (_s[1][3]*_s[2][2]*_s[3][0] - _s[1][2]*_s[2][3]*_s[3][0] - _s[1][3]*_s[2][0]*_s[3][2] + _s[1][0]*_s[2][3]*_s[3][2] + _s[1][2]*_s[2][0]*_s[3][3] - _s[1][0]*_s[2][2]*_s[3][3])/det;
	_d[1][1] = (_s[0][2]*_s[2][3]*_s[3][0] - _s[0][3]*_s[2][2]*_s[3][0] + _s[0][3]*_s[2][0]*_s[3][2] - _s[0][0]*_s[2][3]*_s[3][2] - _s[0][2]*_s[2][0]*_s[3][3] + _s[0][0]*_s[2][2]*_s[3][3])/det;
	_d[1][2] = (_s[0][3]*_s[1][2]*_s[3][0] - _s[0][2]*_s[1][3]*_s[3][0] - _s[0][3]*_s[1][0]*_s[3][2] + _s[0][0]*_s[1][3]*_s[3][2] + _s[0][2]*_s[1][0]*_s[3][3] - _s[0][0]*_s[1][2]*_s[3][3])/det;
	_d[1][3] = (_s[0][2]*_s[1][3]*_s[2][0] - _s[0][3]*_s[1][2]*_s[2][0] + _s[0][3]*_s[1][0]*_s[2][2] - _s[0][0]*_s[1][3]*_s[2][2] - _s[0][2]*_s[1][0]*_s[2][3] + _s[0][0]*_s[1][2]*_s[2][3])/det;
	_d[2][0] = (_s[1][1]*_s[2][3]*_s[3][0] - _s[1][3]*_s[2][1]*_s[3][0] + _s[1][3]*_s[2][0]*_s[3][1] - _s[1][0]*_s[2][3]*_s[3][1] - _s[1][1]*_s[2][0]*_s[3][3] + _s[1][0]*_s[2][1]*_s[3][3])/det;
	_d[2][1] = (_s[0][3]*_s[2][1]*_s[3][0] - _s[0][1]*_s[2][3]*_s[3][0] - _s[0][3]*_s[2][0]*_s[3][1] + _s[0][0]*_s[2][3]*_s[3][1] + _s[0][1]*_s[2][0]*_s[3][3] - _s[0][0]*_s[2][1]*_s[3][3])/det;
	_d[2][2] = (_s[0][1]*_s[1][3]*_s[3][0] - _s[0][3]*_s[1][1]*_s[3][0] + _s[0][3]*_s[1][0]*_s[3][1] - _s[0][0]*_s[1][3]*_s[3][1] - _s[0][1]*_s[1][0]*_s[3][3] + _s[0][0]*_s[1][1]*_s[3][3])/det;
	_d[2][3] = (_s[0][3]*_s[1][1]*_s[2][0] - _s[0][1]*_s[1][3]*_s[2][0] - _s[0][3]*_s[1][0]*_s[2][1] + _s[0][0]*_s[1][3]*_s[2][1] + _s[0][1]*_s[1][0]*_s[2][3] - _s[0][0]*_s[1][1]*_s[2][3])/det;
	_d[3][0] = (_s[1][2]*_s[2][1]*_s[3][0] - _s[1][1]*_s[2][2]*_s[3][0] - _s[1][2]*_s[2][0]*_s[3][1] + _s[1][0]*_s[2][2]*_s[3][1] + _s[1][1]*_s[2][0]*_s[3][2] - _s[1][0]*_s[2][1]*_s[3][2])/det;
	_d[3][1] = (_s[0][1]*_s[2][2]*_s[3][0] - _s[0][2]*_s[2][1]*_s[3][0] + _s[0][2]*_s[2][0]*_s[3][1] - _s[0][0]*_s[2][2]*_s[3][1] - _s[0][1]*_s[2][0]*_s[3][2] + _s[0][0]*_s[2][1]*_s[3][2])/det;
	_d[3][2] = (_s[0][2]*_s[1][1]*_s[3][0] - _s[0][1]*_s[1][2]*_s[3][0] - _s[0][2]*_s[1][0]*_s[3][1] + _s[0][0]*_s[1][2]*_s[3][1] + _s[0][1]*_s[1][0]*_s[3][2] - _s[0][0]*_s[1][1]*_s[3][2])/det;
	_d[3][3] = (_s[0][1]*_s[1][2]*_s[2][0] - _s[0][2]*_s[1][1]*_s[2][0] + _s[0][2]*_s[1][0]*_s[2][1] - _s[0][0]*_s[1][2]*_s[2][1] - _s[0][1]*_s[1][0]*_s[2][2] + _s[0][0]*_s[1][1]*_s[2][2])/det;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type find_max(const float_type a[_Rows][_Cols]) {
	const float_type* _a = &a[0][0];
	const float_type* _ed = _a + _Rows*_Cols;
	float_type r = -std::numeric_limits<float_type>::max();
	for (; _a < _ed; _a++)
		r = Math::max(*_a, r);
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline float_type find_min(const float_type a[_Rows][_Cols]) {
	const float_type* _a = &a[0][0];
	const float_type* _ed = _a + _Rows*_Cols;
	float_type r = std::numeric_limits<float_type>::max();
	for (; _a < _ed; _a++)
		r = Math::min(*_a, r);
	return r;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline bool is_matrix_greater_than(const float_type a[_Rows][_Cols], const float_type b[_Rows][_Cols]) {
	const float_type* _b = &b[0][0];
	float_type maxa = find_max<float_type, _Rows, _Cols>(a);
	const float_type* _ed = _b + _Rows*_Cols;
	for (; _b < _ed; _b++)
		if (maxa <= *_b) return false;
	return true;
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline bool is_matrix_less_than(const float_type a[_Rows][_Cols], const float_type b[_Rows][_Cols]) {
	const float_type* _b = &b[0][0];
	float_type mina = find_min<float_type, _Rows, _Cols>(a);
	const float_type* _ed = _b + _Rows*_Cols;
	for (; _b < _ed; _b++)
		if (mina >= *_b) return false;
	return true;
}

template<typename float_type, size_t rows, size_t cols>
inline bool is_ident(const float_type m[rows][cols]) {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			if (i==j && m[i][j] == 0) return false;
			if (i!=j && m[i][j] != 0) return false;
		}
	}
	return true;
}

template<typename float_type, size_t rows, size_t cols>
inline void set_identy(float_type m[rows][cols]) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			if (i == j) m[i][j] = 1;
			else m[i][j] = 0;
}

template<typename float_type, size_t rows, size_t cols>
inline void set_rotation(float_type m[rows][cols],float_type x,float_type y,float_type z) {
	if (rows == cols && cols > 2) {
		set_identy<float_type,rows,cols>(m);
		float_type ca = Math::cos(x);
		float_type sa = Math::sin(x);
		float_type cb = Math::cos(y);
		float_type sb = Math::sin(y);
		float_type cg = Math::cos(z);
		float_type sg = Math::sin(z);
		m[0][0] = cb*cg;
		m[0][1] = -cb*sg;
		m[0][2] = -sb;
		m[1][0] = ca*sg + sa*sb*cg;
		m[1][1] = ca*cg - sa*sb*sg;
		m[1][2] = -sa*cb;
		m[2][0] = sa*sg - ca*sb*cg;
		m[2][1] = sa*cg + ca*sb*sg;
		m[2][2] = ca*cb;
	} else
		throw math_exception("Matrix must have more than 2 rows and columns and must be square.");
}

template<typename float_type, size_t rows, size_t cols>
inline void set_rotation(float_type m[rows][cols],float_type a) {
	if (rows == cols && cols == 2) {
		m[0][0] = Math::cos(a);
		m[0][1] = -Math::sin(a);
		m[1][0] = Math::sin(a);
		m[1][1] = Math::cos(a);
	} else
		throw math_exception("Matrix must be 2x2.");
}

template<typename float_type, size_t rows, size_t cols>
inline float_type max_at_row(const float_type m[rows][cols], size_t row) {
	float_type g = 0;
	for (size_t j = 0; j < cols; j++)
		if (Math::abs(m[row][j]) > Math::abs(g))
			g = m[row][j];
	return g;
}

template<typename float_type, size_t rows, size_t cols>
inline void mul_and_add_rows(float_type m[rows][cols], size_t row_s, size_t row_d, float_type f) {
	for (size_t j = 0; j < cols; j++ ) {
		m[row_d][j] += f*m[row_s][j];
	}
}

template<typename float_type, size_t rows, size_t cols>
inline void mul_row(float_type a[rows][cols], size_t i, float_type b) {
	for (size_t j = 0; j < cols; j++) {
		a[i][j] *= b;
	}
}

template<typename float_type, size_t rows, size_t cols>
inline void check(float_type m[rows][cols]) {
	for (size_t i = 0; i < rows-1; i++)
		for (size_t j = i+1; j < rows; j++) {
			float_type f = m[i][0]/m[j][0];
			for (size_t k = 1; k < cols; k++) {
				if (f != m[i][k]/m[j][k])
					goto next_row;
			}
			throw math_exception("Matrix is not invertible.");
			next_row:;
		}
}

template <typename float_type, size_t _Rows, size_t _Cols>
inline void invert_NxM(float_type _d[_Rows][_Cols], float_type _s[_Rows][_Cols]) {
	set_identy<float_type,_Rows,_Cols>(_d);
	check<float_type,_Rows,_Cols>(_s);
	while (!is_ident<float_type,_Rows,_Cols>(_s)) {
		for (size_t i = 0; i < _Rows; i++) {
			for (size_t j = 0; j < _Cols; j++) {
				if (i != j) {
					for (size_t k = 0; k < _Rows; k++)
					if (k != i && _s[k][j] != 0) {
						float_type f = _s[i][j]/_s[k][j];
						if (f != 0) {
							float_type d1 = _s[i][j]-f*_s[k][j];
							float_type d2 = _s[i][i]-f*_s[k][i];
							if (d1 == 0 && d2 != 0) {
								mul_and_add_rows<float_type,_Rows,_Cols>(_s,k,i,-f);
								mul_and_add_rows<float_type,_Rows,_Cols>(_d,k,i,-f);
							}
						}
					}
				}
			}
		}
	}
	for (size_t i = 0; i < _Rows; i++) {
		float_type g = 1/max_at_row<float_type,_Rows,_Cols>(_s,i);
		mul_row<float_type,_Rows,_Cols>(_s, i, g);
		mul_row<float_type,_Rows,_Cols>(_d, i, g);
	}
}

}

}

}

#endif

