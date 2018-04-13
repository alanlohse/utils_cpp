/*
 * calculus.hpp
 *
 *  Created on: 11/08/2014
 *      Author: Alan
 */

#ifndef CALCULUS_HPP_
#define CALCULUS_HPP_

#include <utils/math/math.hpp>

namespace utils {

namespace math {

class integral {
private:
	int _M_precision;
public:
	integral() :
			_M_precision(1) {
	}
	integral(int precision) :
			_M_precision(precision) {
	}

	~integral() {
	}

	template <typename _Float, class _Function1D>
	_Float integral_1D(_Function1D& f, _Float x0, _Float x1) {
		_Float r = 0;
		_Float h = (x1 - x0) / _M_precision;
		for (int i = 0; i < _M_precision; i++) {
			_Float x = x0 + (i + _Float(0.5)) * h;
			r += f(x);
		}
		return h * r;
	}

	template <typename _Float, class _Function2D>
	_Float integral_2D(_Function2D& f, _Float x0, _Float x1, _Float y0, _Float y1) {
		_Float r = 0;
		_Float h = (x1 - x0) / _M_precision;
		_Float v = (y1 - y0) / _M_precision;
		for (int j = 0; j < _M_precision; j++) {
			_Float y = y0 + (j + _Float(0.5)) * v;
			for (int i = 0; i < _M_precision; i++) {
				_Float x = x0 + (i + _Float(0.5)) * h;
				r += f(x, y);
			}
		}
		return h * v * r;
	}

	template <typename _Float, class _Function3D>
	_Float integral_3D(_Function3D& f, _Float x0, _Float x1, _Float y0, _Float y1, _Float z0, _Float z1) {
		_Float r = 0;
		_Float h = (x1 - x0) / _M_precision;
		_Float v = (y1 - y0) / _M_precision;
		_Float w = (z1 - z0) / _M_precision;
		for (int k = 0; k < _M_precision; k++) {
			_Float z = z0 + (k + _Float(0.5)) * w;
			for (int j = 0; j < _M_precision; j++) {
				_Float y = y0 + (j + _Float(0.5)) * v;
				for (int i = 0; i < _M_precision; i++) {
					_Float x = x0 + (i + _Float(0.5)) * h;
					r += f(x, y, z);
				}
			}
		}
		return h * v * w * r;
	}

	template <typename _Float, class _Function1D>
	_Float derivative_1D(_Function1D& f, _Float x) {
		_Float h = _Float(1) / _M_precision;
		return (f(x + h) - f(x - h)) / (2 * h);
	}

	template <typename _Float, class _Function2D>
	_Float derivative_2Dx(_Function2D& f, _Float x, _Float y) {
		_Float h = _Float(1) / _M_precision;
		return (f(x + h, y) - f(x - h, y)) / (2 * h);
	}

	template <typename _Float, class _Function2D>
	_Float derivative_2Dy(_Function2D& f, _Float x, _Float y) {
		_Float h = _Float(1) / _M_precision;
		return (f(x, y + h) - f(x, y - h)) / (2 * h);
	}

	template <typename _Float, class _Function3D>
	_Float derivative_3Dx(_Function3D& f, _Float x, _Float y, _Float z) {
		_Float h = _Float(1) / _M_precision;
		return (f(x + h, y, z) - f(x - h, y, z)) / (2 * h);
	}

	template <typename _Float, class _Function3D>
	_Float derivative_3Dy(_Function3D& f, _Float x, _Float y, _Float z) {
		_Float h = _Float(1) / _M_precision;
		return (f(x, y + h, z) - f(x, y - h, z)) / (2 * h);
	}

	template <typename _Float, class _Function3D>
	_Float derivative_3Dz(_Function3D& f, _Float x, _Float y, _Float z) {
		_Float h = _Float(1) / _M_precision;
		return (f(x, y, z + h) - f(x, y, z - h)) / (2 * h);
	}

};

} // math

} // utils

#endif /* CALCULUS_HPP_ */
