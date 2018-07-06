/*
 * test_main.cpp
 *
 *  Created on: 30/03/2010
 *      Author: alan.lohse
 */

#include <utils/utils_defs.hpp>
#include <utils/memory/memory_pool.hpp>
#undef DEFAULT_ALLOCATOR
#define DEFAULT_ALLOCATOR utils::memory::pooled_allocator

#include <utils/test/test_tools.hpp>
#include <iostream>
#include <utils/thread/thread.hpp>
#include <utils/typeutils.hpp>

using namespace std;
using namespace utils;

void register_test_commons();
void register_test_containers();
void register_test_net();
void test_memory_pool();
void test_geom();
void test_math();
void test1();
void register_test_codec();

struct ftest {
	void operator()() {
		for (int i = 0; i < 10; i++) {
			cout << "test" << endl;
			utils::thread::sleep(100);
		}
	}
};

int main(int argc, char **argv) {
	test_geom();
	test_math();
	register_test_commons();
	register_test_containers();
	register_test_net();
	register_test_codec();
	test1();
//	ftest fc;
//	utils::thread tb(fc);
//	tb.start();
//	test_memory_pool();
//	test_memory_pool();

//	cout << "Wating for other thread." << endl;
//	tb.join();
	cout << "Starting tests" << endl;
	return utils::test::TestSuit::run_tests();
}
