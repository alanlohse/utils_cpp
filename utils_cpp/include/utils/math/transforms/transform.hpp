/*
 * transform.hpp
 *
 *  Created on: 30/01/2014
 *      Author: Alan
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/math/math_exception.hpp>
#include <complex>

namespace utils {

namespace math {

namespace transforms {

template<typename _Float>
class Transform {
public:
	typedef std::complex<_Float> complex_type;
	typedef _Float float_type;
public:
	virtual ~Transform() { }

	virtual const complex_type f(const float_type& x) = 0;

	virtual float_type getFundamentalFrequency() = 0;

	virtual void normalize() = 0;

	virtual void lowPass(float_type cutoff, float_type w) = 0;

	virtual void highPass(float_type cutoff, float_type w) = 0;

	virtual void bandPass(float_type center, float_type w) = 0;

	virtual size_t size() = 0;

	virtual complex_type coefficient(size_t i) = 0;

};

}

}

}


#endif /* TRANSFORM_HPP_ */
