/*
 * test_math_geom.cpp
 *
 *  Created on: 19/07/2014
 *      Author: Alan
 */

#include <utils/math/geometry/vectors.hpp>
#include <utils/math/geometry/matrix.hpp>
#include <utils/math/functions/calculus.hpp>
#include <utils/math/geometry/geometry.hpp>
#include <utils/math/error.hpp>
#include <utils/test/test_tools.hpp>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace utils;
using namespace utils::math;

class Test_Vector : public utils::test::TestCase {
public:

	Test_Vector() {
		ADD_TEST_FUNCTION(Test_Vector, Test_Vector::test_access);
		ADD_TEST_FUNCTION_EX(Test_Vector, Test_Vector::test_access_ex,"Index out of range.");
		ADD_TEST_FUNCTION(Test_Vector, Test_Vector::test_add);
		ADD_TEST_FUNCTION(Test_Vector, Test_Vector::test_mul);
		ADD_TEST_FUNCTION(Test_Vector, Test_Vector::test_cross);
		ADD_TEST_FUNCTION(Test_Vector, Test_Vector::test_length);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_access() {
		utils::math::vector<double,3> a(1.0,2.0,3.0);
		ASSERT2(a[0],1.0);
		ASSERT2(a[1],2.0);
		ASSERT2(a[2],3.0);
		a[2] += 1.2;
		ASSERT2(a[2],4.2);
	}

	void test_access_ex() {
		utils::math::vector<double,3> a(1.0,2.0,3.0);
		a[3] = 4.0;
	}

	void test_add() {
		utils::math::vector<double,3> a(1.0,1.0,0.0);
		utils::math::vector<long double,3> b(0.0L,1.0L,1.0L);
		utils::math::vector<double,3> c(1.0,2.0,1.0);
		utils::math::vector<double,3> d = a + b;
		ASSERT2((d == c),true);
		utils::math::vector<double,3> e(1.0,0.0,-1.0);
		utils::math::vector<double,3> f = a - b;
		ASSERT2((e == f),true);
	}

	void test_mul() {
		utils::math::vector<double,3> a(1.0,1.0,0.0);
		utils::math::vector<long double,3> b(0.0L,1.0L,1.0L);
		double c = a * b;
		ASSERT2((c == 1),true);
		utils::math::vector<double,3> d = a * 2.0;
		utils::math::vector<double,3> e(2.0,2.0,0.0);
		ASSERT2((e == d),true);
	}

	void test_cross() {
		utils::math::vector<double,3> a(1.0,0.0,0.0);
		utils::math::vector<long double,3> b(0.0L,1.0L,0.0L);
		utils::math::vector<double,3> d = a & b;
		utils::math::vector<double,3> e(0.0,0.0,1.0);
		ASSERT2((d == e),true);
	}

	void test_length() {
		utils::math::vector<double,3> a(3.0,4.0,0.0);
		ASSERT2((a.length() == 5),true);
	}

};

class Test_Matrix : public utils::test::TestCase {
public:

	matrix<double,5,5> m;
	matrix<double,5,5> im;

	Test_Matrix() {
		ADD_TEST_FUNCTION(Test_Matrix, Test_Matrix::test_add);
		ADD_TEST_FUNCTION(Test_Matrix, Test_Matrix::test_mul);
		ADD_TEST_FUNCTION(Test_Matrix, Test_Matrix::test_determinant);
		ADD_TEST_FUNCTION(Test_Matrix, Test_Matrix::test_inverse);
	}

	virtual void prepare_test() {
		m = matrix<double,5,5>(1.0,2.0,3.0,4.0,5.0,
				3.0,2.0,3.0,4.0,5.0,
				4.0,3.0,3.0,4.0,5.0,
				5.0,3.0,0.0,4.0,5.0,
				2.0,0.0,0.0,0.0,5.0);
		im = matrix<double,5,5>(-1.0/2,1.0/2,0.0,0.0,0.0,
				1/2.0,-3/2.0,1.0,0.0,0.0,
				-1/6.0,1/6.0,1/3.0,-1/3.0,0.0,
				0.0,3/4.0,-3/4.0,1/4.0,-1/4.0,
				1/5.0,-1/5.0,0.0,0.0,1/5.0);
	}
	virtual void close_test() { }

	void test_add() {
		matrix<double,4,4> a(1.0, 2.0, 3.0, 4.0,
				5.0, 6.0, 7.0, 8.0,
				9.0, 1.0, 2.0, 3.0,
				4.0, 5.0, 6.0, 7.0);
		matrix<double,4,4> b(9.0, 8.0, 7.0, 6.0,
				5.0, 4.0, 3.0, 2.0,
				1.0, 9.0, 8.0, 7.0,
				6.0, 5.0, 4.0, 3.0);
		matrix<double,4,4> i(
				10.0,10.0,10.0,10.0,
				10.0,10.0,10.0,10.0,
				10.0,10.0,10.0,10.0,
				10.0,10.0,10.0,10.0);
		ASSERT2((a + b), i);
	}

	void test_mul() {
		matrix<double,4,4> a(1.0, 2.0, 3.0, 4.0,
				5.0, 6.0, 7.0, 8.0,
				9.0, 1.0, 2.0, 3.0,
				4.0, 5.0, 6.0, 7.0);
		matrix<double,4,4> b(9.0, 8.0, 7.0, 6.0,
				5.0, 4.0, 3.0, 2.0,
				1.0, 9.0, 8.0, 7.0,
				6.0, 5.0, 4.0, 3.0);
		matrix<double,4,4> i(
				46.0,63.0,53.0,43.0,
				130.0,167.0,141.0,115.0,
				106.0,109.0,94.0,79.0,
				109.0,141.0,119.0,97.0);
		a = (a * b);
		ASSERT2(a, i);
	}

	void test_determinant() {
		double d = m.determinant();
		ASSERT2(d,120);
	}

	void test_inverse() {
		matrix<double,2,2> a(4.0, 1.0, 3.0, 2.0);
		matrix<double,2,2> b(0.4,-0.2,-0.6, 0.8);
		ASSERT2(a.inverse(),b);
		matrix<double,4,4> c(1.0, 2.0, 3.0, 4.0,
				6.0, 5.0, 3.0, 4.0,
				0.0, 0.0, 3.0, 4.0,
				4.0, 3.0, 1.0, 2.0);
		matrix<double,4,4> d(-5/7.0,2/7.0,3/7.0,0.0,
				6/7.0,-1/7.0,-5/7.0,0.0,
				-4/7.0,10/7.0,1/7.0,-2.0,
				3/7.0,-15/14.0,1/7.0,3/2.0);
		ASSERT2(c.inverse(),d);
		ASSERT2(~m,im);
	}

};


void test_geom(){
	ADD_TEST_CASE(Test_Vector);
	ADD_TEST_CASE(Test_Matrix);
	utils::test::TestSuit a;
	a.run_tests();
	errored_number<double> an = 10.0;
	errored_number<double> bn = 12.0;
	cout << an << " * " << bn << " = " << (an * bn);
}

