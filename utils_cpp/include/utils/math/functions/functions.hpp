/*
 * functions.hpp
 *
 *  Created on: 11/08/2014
 *      Author: Alan
 */

#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <utils/math/math.hpp>

namespace utils {

namespace math {

template <typename _Float>
class function1D {
public:
	typedef _Float float_type;

	double operator()(float_type x) {
		return x;
	}

};

template <typename _Float>
class function2D {
public:
	typedef _Float float_type;

	double operator()(float_type x, float_type y) {
		return x + y;
	}

};

template <typename _Float>
class function3D {
public:
	typedef _Float float_type;

	double operator()(float_type x, float_type y, float_type z) {
		return x + y + z;
	}

};

} // math

} // utils

#endif /* FUNCTIONS_HPP_ */
