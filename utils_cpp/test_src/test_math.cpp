/*
 * test_math_geom.cpp
 *
 *  Created on: 19/07/2014
 *      Author: Alan
 */

#include <utils/math/geometry/vectors.hpp>
#include <utils/math/geometry/matrix.hpp>
#include <utils/math/functions/calculus.hpp>
#include <utils/math/transforms/interpolation.hpp>
#include <utils/math/error.hpp>
#include <utils/test/test_tools.hpp>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace utils;
using namespace utils::math;
using namespace utils::math::interpolation;

class Test_Interpolation : public utils::test::TestCase {
public:

	Test_Interpolation() {
		ADD_TEST_FUNCTION(Test_Interpolation, Test_Interpolation::compare_interps);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void compare_interps() {
		double ys[] = {0,1,-1,2,0,-2,0,1,0};
		double* begin = ys;
		int N = sizeof(ys)/sizeof(double);
		double* end = begin+N;
		LinearInterpolator<double,double*> linear = LinearInterpolator<double,double*>(begin,end);
		QuadraticInterpolator<double,double*> quad = QuadraticInterpolator<double,double*>(begin,end);
		CubicInterpolator<double,double*> cubi = CubicInterpolator<double,double*>(begin,end);
		LanczosInterpolator<double,double*,2> lanc = LanczosInterpolator<double,double*,2>(begin,end);
		for (int i = 0; i < N*100; i++) {
			double x = i/100.0;
			cout << x << "\t" << linear.get(x) << "\t" << quad.get(x) << "\t" << cubi.get(x) << "\t" << lanc.get(x) << std::endl;
		}
	}

};


void test_math(){
	ADD_TEST_CASE(Test_Interpolation);
	utils::test::TestSuit a;
	a.run_tests();
}

