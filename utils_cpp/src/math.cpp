/*
 * math.cpp
 *
 *  Created on: 16/08/2014
 *      Author: Alan
 */

#include <utils/math/error.hpp>

namespace utils {

namespace math {


double error_measure::double_min_alg = ldexp(1,-55);
double error_measure::float_min_alg = ldexp(1,-23);
double error_measure::long_double_min_alg = ldexp(1,-63);

}

}
