/*
 * test_tools.hpp
 *
 *  Created on: 30/03/2010
 *      Author: alan.lohse
 */

#ifndef TEST_TOOLS_HPP_
#define TEST_TOOLS_HPP_

#include <utils/utils_defs.hpp>
#include <utils/time.hpp>
#include <utils/functions/bound_funcs.hpp>
#include <vector>
#include <iostream>
#include <utils/exception.hpp>
#include <string>
#include <cstring>

namespace utils {

namespace test {

struct test_function {
	functions::bound_func* address;
	const char* name;
	const char* exception_message;
	test_function(functions::bound_func* _address, const char* _name, const char* _exception_message) {
		address = _address;
		name = _name;
		exception_message = _exception_message;
	}
};

#define ADD_TEST_FUNCTION(cl,fn) tests.push_back(utils::test::test_function(new utils::functions::bound_func_NP<void,cl>((utils::functions::bound_func_NP<void,cl>::func_type)&fn),(const char*)#fn,NULL));
#define ADD_TEST_FUNCTION_EX(cl,fn,excp) tests.push_back(utils::test::test_function(new utils::functions::bound_func_NP<void,cl>((utils::functions::bound_func_NP<void,cl>::func_type)&fn),(const char*)#fn,(const char*)excp));

class test_exception : public utils_exception {
public:
	test_exception() throw() :
		utils_exception()
	{
	}
	test_exception(const std::string& cause) throw() :
		utils_exception(cause)
	{
	}
};

#define ASSERT2(value,expected) assert_test(value == expected, "expected " #expected " but found " #value)
#define ASSERT(value,message) assert_test(value, message)
#define ASSERT3(value) assert_test(value, #value)

class TestCase {
protected:
	std::vector<test_function> tests;
	friend class TestSuit;
	void assert_test(bool tested, const std::string& message) {
		if (!tested) throw test_exception(message);
	}
public:
	virtual ~TestCase() {
	}
	virtual void prepare_test() = 0;
	virtual void close_test() = 0;
};

struct test_case {
	TestCase* tcase;
	const char* name;
	test_case(TestCase* _tcase, const char* _name) {
		tcase = _tcase;
		name = _name;
	}
	test_case(const test_case& other) {
		tcase = other.tcase;
		name = other.name;
	}
};

#define ADD_TEST_CASE(tc) utils::test::TestSuit::add_test(utils::test::test_case(new tc(),#tc));

class TestSuit {
private:
	static std::vector<test_case>& _tests() {
		static std::vector<test_case>* tests;
		if (tests == NULL) tests = new std::vector<test_case>();
		return *tests;
	}
public:
	static void add_test(const test_case& tc) {
		_tests().push_back(tc);
	}

	static int run_tests() {
		for (std::vector<test_case>::iterator it = _tests().begin(), end = _tests().end();
				it != end; it++) {
			std::cout << "Running test case " << it->name << std::endl;
			int success_count = 0, failed_count = 0;
			t_bigint start_all, end_all;
			start_all = Time::milliseconds();
			for (std::vector<test_function>::iterator it2 = it->tcase->tests.begin(), end2 = it->tcase->tests.end();
					it2 != end2; it2++) {
				std::cout << "Test method: " << it2->name << " ... ";
				long start, finish;
				it->tcase->prepare_test();
				bool success = it2->exception_message == NULL;
				std::string message = "ok";
				start = Time::nanoseconds();
				try {
					it2->address->call(NULL,it->tcase,NULL);
				} catch(std::exception& exc) {
					success = it2->exception_message != NULL && strcmp(it2->exception_message,exc.what()) == 0;
					message = exc.what();
				}
				finish = Time::nanoseconds();
				it->tcase->close_test();
				if (success) {
					std::cout << "SUCCESS";
					success_count++;
				} else {
					std::cout << "FAILURE: " << message;
					failed_count++;
				}
				std::cout << " in " << (finish-start) << " ns" << std::endl;
			}
			end_all = Time::milliseconds();
			std::cout << "Tests done: " << it->tcase->tests.size() << " in " << (end_all - start_all) << " ms " << std::endl;
			std::cout << "Successes: " << success_count << std::endl;
			std::cout << "Failures: " << failed_count << std::endl;
			std::cout << std::endl;
			return failed_count;
		}
	}
};

}

}

#endif /* TEST_TOOLS_HPP_ */
