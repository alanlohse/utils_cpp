/*
 * math.hpp
 *
 *  Created on: 01/02/2014
 *      Author: Alan
 */

#ifndef MATH_HPP_
#define MATH_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/math/math_exception.hpp>
#include <cmath>
#include <complex>

namespace utils {

namespace math {

class Math {
public:

	template<typename F>
	static F ceil(const F& x) { return x; }

	template<typename F>
	static F floor(const F& x) { return x; }

	template<typename F>
	static F abs(const F& x) { return x < 0 ? -x : x; }

	template<typename F>
	static F cos(const F& x) { return x; }

	template<typename F>
	static F sin(const F& x) { return x; }

	template<typename F>
	static F tan(const F& x) { return x; }

	template<typename F>
	static F acos(const F& x) { return x; }

	template<typename F>
	static F asin(const F& x) { return x; }

	template<typename F>
	static F atan(const F& x) { return x; }

	template<typename F>
	static F atan2(const F& x, const F& y) { return x; }

	template<typename F>
	static F cosh(const F& x) { return x; }

	template<typename F>
	static F sinh(const F& x) { return x; }

	template<typename F>
	static F sinc(const F& x) {
		if (x == 0) return 1;
		F arg = x*pi(F(1));
		return sin(arg)/arg;
	}

	template<typename F>
	static F tanh(const F& x) { return x; }

	template<typename F>
	static F acosh(const F& x) { return x; }

	template<typename F>
	static F asinh(const F& x) { return x; }

	template<typename F>
	static F atanh(const F& x) { return x; }

	template<typename F>
	static F exp(const F& x) { return x; }

	template<typename F>
	static F ln(const F& x) { return x; }

	template<typename F>
	static F sqr(const F& x) { return x*x; }

	template<typename F>
	static F sqrt(const F& x) { return x; }

	template<typename F>
	static F max(const F& a, const F& b) { return a > b ? a : b; }

	template<typename F>
	static F min(const F& a, const F& b) { return a < b ? a : b; }

	template<typename F>
	static F max(const F& a, const F& b, const F& c) {
		return a > b && a > c ? a : (b > c ? b : c);
	}

	template<typename F>
	static F min(const F& a, const F& b, const F& c) {
		return a < b && a < c ? a : (b < c ? b : c);
	}

	template<typename F>
	static F abs_max(const F& a, const F& b) { return max(abs(a),abs(b)); }

	template<typename F>
	static F abs_min(const F& a, const F& b) { return min(abs(a),abs(b)); }

	template<typename F>
	static F limit(const F& a, const F& max, const F& min) {
		if (a > max) return max;
		if (a < min) return min;
		return a;
	}

	template<typename F>
	static F pi(const F m = 1.0) {
		return m*3.1415926535897932384626433832795028841971693993751058209749445923078164;
	}

};


template<>
inline long double Math::ceil<long double>(const long double& x) {
	return ::ceill(x);
}

template<>
inline double Math::ceil<double>(const double& x) {
	return ::ceil(x);
}

template<>
inline float Math::ceil<float>(const float& x) {
	return ::ceilf(x);
}


template<>
inline long double Math::floor<long double>(const long double& x) {
	return ::floorl(x);
}

template<>
inline double Math::floor<double>(const double& x) {
	return ::floor(x);
}

template<>
inline float Math::floor<float>(const float& x) {
	return ::floorf(x);
}


template<>
inline long double Math::cos<long double>(const long double& x) { return ::cosl(x); }

template<>
inline double Math::cos<double>(const double& x) { return ::cos(x); }

template<>
inline float Math::cos<float>(const float& x) { return ::cosf(x); }

template<>
inline long double Math::sin<long double>(const long double& x) { return ::sinl(x); }

template<>
inline double Math::sin<double>(const double& x) { return ::sin(x); }

template<>
inline float Math::sin<float>(const float& x) { return ::sinf(x); }

template<>
inline long double Math::tan<long double>(const long double& x) { return ::tanl(x); }

template<>
inline double Math::tan<double>(const double& x) { return ::tan(x); }

template<>
inline float Math::tan<float>(const float& x) { return ::tanf(x); }

template<>
inline long double Math::acos<long double>(const long double& x) { return ::acosl(x); }

template<>
inline double Math::acos<double>(const double& x) { return ::acos(x); }

template<>
inline float Math::acos<float>(const float& x) { return ::acosf(x); }

template<>
inline long double Math::asin<long double>(const long double& x) { return ::asinl(x); }

template<>
inline double Math::asin<double>(const double& x) { return ::asin(x); }

template<>
inline float Math::asin<float>(const float& x) { return ::asinf(x); }

template<>
inline long double Math::atan<long double>(const long double& x) { return ::atanl(x); }

template<>
inline double Math::atan<double>(const double& x) { return ::atan(x); }

template<>
inline float Math::atan<float>(const float& x) { return ::atanf(x); }

template<>
inline long double Math::atan2<long double>(const long double& x, const long double& y) { return ::atan2l(x,y); }

template<>
inline double Math::atan2<double>(const double& x, const double& y) { return ::atan2(x,y); }

template<>
inline float Math::atan2<float>(const float& x, const float& y) { return ::atan2f(x,y); }



template<>
inline long double Math::cosh<long double>(const long double& x) { return ::coshl(x); }

template<>
inline double Math::cosh<double>(const double& x) { return ::cosh(x); }

template<>
inline float Math::cosh<float>(const float& x) { return ::coshf(x); }

template<>
inline long double Math::sinh<long double>(const long double& x) { return ::sinhl(x); }

template<>
inline double Math::sinh<double>(const double& x) { return ::sinh(x); }

template<>
inline float Math::sinh<float>(const float& x) { return ::sinhf(x); }

template<>
inline long double Math::tanh<long double>(const long double& x) { return ::tanhl(x); }

template<>
inline double Math::tanh<double>(const double& x) { return ::tanh(x); }

template<>
inline float Math::tanh<float>(const float& x) { return ::tanhf(x); }



template<>
inline long double Math::exp<long double>(const long double& x) { return ::expl(x); }

template<>
inline double Math::exp<double>(const double& x) { return ::exp(x); }

template<>
inline float Math::exp<float>(const float& x) { return ::expf(x); }


template<>
inline long double Math::sqrt<long double>(const long double& x) { return ::sqrtl(x); }

template<>
inline double Math::sqrt<double>(const double& x) { return ::sqrt(x); }

template<>
inline float Math::sqrt<float>(const float& x) { return ::sqrtf(x); }

template<>
inline int Math::abs<int>(const int& x) { return ::abs(x); }

template<>
inline t_bigint Math::abs<t_bigint>(const t_bigint& x) { return ::llabs(x); }

template<>
inline float Math::abs<float>(const float& x) { return ::fabsf(x); }

template<>
inline double Math::abs<double>(const double& x) { return ::fabs(x); }

template<>
inline long double Math::abs<long double>(const long double& x) { return ::fabsl(x); }

template<>
inline long double Math::pi<long double>(const long double m) {
	return m*3.1415926535897932384626433832795028841971693993751058209749445923078164L;
}

}

}

#endif /* MATH_HPP_ */
