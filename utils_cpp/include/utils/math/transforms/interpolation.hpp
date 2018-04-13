/*
 * interpolation.hpp
 *
 *  Created on: 04/02/2014
 *      Author: Alan
 */

#ifndef INTERPOLATION_HPP_
#define INTERPOLATION_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/math/math_exception.hpp>
#include <complex>

namespace utils {

namespace math {

namespace interpolation {

template<typename _Nt>
class Interpolator {
public:
	typedef _Nt float_type;
public:
	virtual ~Interpolator() { }
	virtual float_type get(const float_type& x) = 0;
};

template<typename _Nt>
class Interpolator2D {
public:
	typedef _Nt float_type;
public:
	virtual ~Interpolator2D() { }
	virtual float_type get(const float_type& x, const float_type& y) = 0;
};

template<typename _Nt, typename _IteratorType>
class LinearInterpolator {
public:
	typedef _Nt float_type;
	typedef _IteratorType iterator;
	typedef ptrdiff_t pointer_diff;
private:
	iterator _begin,_end;
public:
	LinearInterpolator(): _begin(),_end() {
	}
	LinearInterpolator(const iterator& begin, const iterator& end): _begin(begin),_end(end) {
	}
	virtual ~LinearInterpolator() { }
	virtual float_type get(const float_type& x) {
		float_type x0 = Math::floor(x);
		pointer_diff i = (pointer_diff) x0;
		if (i < 0) {
			i = 0;
			x0 = 0;
		}
		iterator it = _begin + i;
		if (it >= _end-1) {
			it--;
			x0--;
		}
		float_type y0 = *it;
		float_type y1 = *(it+1);
		return (x-x0)*(y1-y0) + y0;
	}
};

template<typename _Nt, typename _IteratorType>
class QuadraticInterpolator {
public:
	typedef _Nt float_type;
	typedef _IteratorType iterator;
	typedef ptrdiff_t pointer_diff;
private:
	iterator _begin,_end;
public:
	QuadraticInterpolator(): _begin(),_end() {
	}
	QuadraticInterpolator(const iterator& begin, const iterator& end): _begin(begin),_end(end) {
	}
	virtual ~QuadraticInterpolator() { }
	virtual float_type get(const float_type& x) {
		float_type x0 = Math::floor(x);
		pointer_diff i = (pointer_diff) x0;
		if (i < 0) {
			i = 0;
			x0 = 0;
		}
		iterator it = _begin + i;
		if (it >= _end-2) {
			it = _end-3;
			i = it-_begin;
			x0 = i;
		}
		float_type x1 = x0+1;
		float_type x2 = x1+1;
		float_type y0 = *it;
		float_type y1 = *(it+1);
		float_type y2 = *(it+2);
		return (x-x1)*(x-x2)*y0/2 - (x-x0)*(x-x2)*y1 + (x-x0)*(x-x1)*y2/2;
	}
};

template<typename _Nt, typename _IteratorType>
class CubicInterpolator {
public:
	typedef _Nt float_type;
	typedef _IteratorType iterator;
	typedef ptrdiff_t pointer_diff;
private:
	iterator _begin,_end;
public:
	CubicInterpolator(): _begin(),_end() {
	}
	CubicInterpolator(const iterator& begin, const iterator& end): _begin(begin),_end(end) {
	}
	virtual ~CubicInterpolator() { }
	virtual float_type get(const float_type& x) {
		float_type x0 = Math::floor(x);
		pointer_diff i = (pointer_diff) x0;
		if (i < 0) {
			i = 0;
			x0 = 0;
		}
		iterator it = _begin + i;
		if (it >= _end-3) {
			it = _end-4;
			i = it-_begin;
			x0 = i;
		}
		float_type x1 = x0+1;
		float_type x2 = x1+1;
		float_type x3 = x2+1;
		float_type y0 = *it;
		float_type y1 = *(++it);
		float_type y2 = *(++it);
		float_type y3 = *(++it);
		return (x - x1)*(x - x2)*(x - x3)*y0/((-1)*(-2)*(-3))
			  +(x - x0)*(x - x2)*(x - x3)*y1/((1)*(-1)*(-2))
			  +(x - x0)*(x - x1)*(x - x3)*y2/((2)*(1)*(-1))
			  +(x - x0)*(x - x1)*(x - x2)*y3/((3)*(2)*(1));
	}
};

template<typename _Nt, typename _IteratorType, int _A = 2>
class LanczosInterpolator {
public:
	typedef _Nt float_type;
	typedef _IteratorType iterator;
	typedef ptrdiff_t pointer_diff;
private:
	iterator _begin,_end;
	float_type lanc(float_type x) {
		if (x < _A && x > -_A)
			return Math::sinc(x)*Math::sinc(x/_A);
		else
			return 0;
	}
public:
	LanczosInterpolator(): _begin(),_end() {
	}
	LanczosInterpolator(const iterator& begin, const iterator& end): _begin(begin),_end(end) {
	}
	virtual ~LanczosInterpolator() { }
	virtual float_type get(const float_type& x) {
		float_type x0 = Math::floor(x);
		int n = (int) x0;
		float_type r = 0;
		int i = n - (_A - 1);
		int size = _end - _begin;
		iterator last = _begin + (size-1);
		iterator it = _begin + i;
		for (; i <= n + _A; i++, it++) {
			if (i < 0)
				r += (*_begin)*lanc(x - x0 - (i - n));
			else if (i < size)
				r += (*it)*lanc(x - x0 - (i - n));
			else
				r += (*last)*lanc(x - x0 - (i - n));
		}
		return r;
	}
};


template<typename _Nt, typename _InputFunction>
class BilinearInterpolator {
public:
	typedef _Nt float_type;
	typedef _InputFunction input_function_type;
private:
	input_function_type _input;
public:
	BilinearInterpolator(): _input() {
	}
	BilinearInterpolator(const input_function_type& input): _input(input) {
	}
	virtual ~BilinearInterpolator() { }
	virtual float_type get(const float_type& x, const float_type& y) {
		int x0 = (int) Math::floor(x);
		int y0 = (int) Math::floor(y);
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		float_type dx = x-x0;
		float_type dy = y-y0;
		float_type z00 = _input(x0,y0);
		float_type z01 = _input(x0,y1);
		float_type z10 = _input(x1,y0);
		float_type z11 = _input(x1,y1);
		return (1-dx)*(1-dy)*z00 + dx*(1-dy)*z10 + (1-dx)*dy*z01 + dx*dy*z11;
	}
};

template<typename _Nt, typename _InputFunction>
class BicubicInterpolator {
public:
	typedef _Nt float_type;
	typedef _InputFunction input_function_type;
private:
	input_function_type _input;
	float_type cubic1(float_type p[], float_type x) {
		float_type a = p[3] / 6 - p[2] / 2 + p[1] / 2 - p[0] / 6;
		float_type b = (p[2] - 2 * p[1] + p[0]) / 2 - 3 * a;
		float_type d = p[0];
		float_type c = p[1] - p[0] - (a + b);
		return a * x * x * x + b * x * x + c * x + d;
	}

	float_type cubic(float_type p[4][4], float_type x, float_type y) {
		float_type arr[] = { 0, 0, 0, 0 };
		arr[0] = cubic1(p[0], y);
		arr[1] = cubic1(p[1], y);
		arr[2] = cubic1(p[2], y);
		arr[3] = cubic1(p[3], y);
		return cubic1(arr, x);
	}

public:
	BicubicInterpolator(): _input() {
	}
	BicubicInterpolator(const input_function_type& input): _input(input) {
	}
	virtual ~BicubicInterpolator() { }
	virtual float_type get(const float_type& x, const float_type& y) {
		int x0 = (int) Math::floor(x);
		int y0 = (int) Math::floor(y);
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		float_type dx = x-x0;
		float_type dy = y-y0;
		float_type p[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				p[i][j] = _input(x0+i-1, y0+j-1);
		return cubic(p, dx, dy);
	}
};

template<typename _Nt, typename _InputFunction, int _A = 2>
class Lanczos2DInterpolator {
public:
	typedef _Nt float_type;
	typedef _InputFunction input_function_type;
private:
	input_function_type _input;
	float_type lanc(float_type x) {
		if (x < _A && x > -_A)
			return Math::sinc(x)*Math::sinc(x/_A);
		else
			return 0;
	}
public:
	Lanczos2DInterpolator(): _input() {
	}
	Lanczos2DInterpolator(const input_function_type& input): _input(input) {
	}
	virtual ~Lanczos2DInterpolator() { }
	virtual float_type get(const float_type& x, const float_type& y) {
		float_type x0 = Math::floor(x);
		float_type y0 = Math::floor(y);
		float_type r = 0;
		int i = (int) x0 - (_A - 1);
		int j = (int) y0 - (_A - 1);
		int fi = (int) x0 + _A;
		int fj = (int) y0 + _A;
		for (; i < fi; i++)
			for (; j < fj; j++)
				r += _input(i, j) * lanc(x - i) * lanc(y - j);
		return r;
	}
};

}

}

}

#endif /* INTERPOLATION_HPP_ */
