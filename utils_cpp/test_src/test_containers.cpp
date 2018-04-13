/*
 * test_containers.cpp
 *
 *  Created on: 01/04/2010
 *      Author: alan.lohse
 */

#include <utils/time.hpp>
#include <utils/funcs.hpp>
#include <utils/test/test_tools.hpp>
#include <utils/containers/array.hpp>
#include <utils/containers/hash_map.hpp>
#include <utils/containers/simple_list.hpp>
#include <utils/containers/stream.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace utils;
using namespace utils::containers;

class Test_array : public utils::test::TestCase {
public:

	Test_array() {
		ADD_TEST_FUNCTION(Test_array, Test_array::test_empty);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_empty() {
		utils::containers::array<int,1> a(0);
		ASSERT2(a.empty(),true);
		bool excep = false;
		try {
			a[0] = 0;
		} catch(boundary_exceeded_exception& exc) {
			excep = true;
		}
		ASSERT2(excep,true);
	}

};

struct funcc {
			void operator()(hashmap<int,int>& x) {
				x.size();
			}

		};
class Test_hash_map : public utils::test::TestCase {
public:

	Test_hash_map() {
		ADD_TEST_FUNCTION(Test_hash_map, Test_hash_map::test_insert);
		ADD_TEST_FUNCTION(Test_hash_map, Test_hash_map::test_erase);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_insert() {
		hashmap<int,int> hm;
		ASSERT2(hm.size(),0);
		for (int i = 0;i < 100;i++)
			hm.insert(pair<int,int>(i,i*3+2));
		for (int i = 0;i < 100;i++)
			ASSERT2(hm.find(i)->second,i*3+2);
		ASSERT2(hm.size(),100);
		funcc f;
		hm.sync(f);
	}

	void test_erase() {
		hashmap<int,int> hm;
		ASSERT2(hm.size(),0);
		for (int i = 0;i < 200;i++)
			hm.insert(pair<int,int>(i,i*3+2));
		ASSERT2(hm.size(),200);
		for (int i = 50;i < 150;i++)
			hm.erase(i);
		ASSERT2(hm.size(),100);
//		hm.erase(hm.begin(),hm.begin()+50);
		ASSERT2(hm.size(),50);
	}
};

class Test_fwdlist : public utils::test::TestCase {
public:

	Test_fwdlist() {
		ADD_TEST_FUNCTION(Test_fwdlist, Test_fwdlist::test_pushpop);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_pushpop() {
		fwdlist<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		list.push_back(5);
		list.push_back(6);
		list.push_back(7);
		list.push_front(0);
		fwdlist<int>::iterator it = list.begin();
		for (; it; ++it)
			cout << *it;
		cout << endl;
		list.pop_back();
		it = list.begin();
		for (; it; ++it)
			cout << *it;
		cout << endl;
		list.pop_front();
		it = list.begin();
		for (; it; ++it)
			cout << *it;
		cout << endl;
		list.remove(4);
		list.remove(8);
		it = list.begin();
		for (; it; ++it)
			cout << *it;
		cout << endl;
	}

};

struct __pair_filter__ {
	bool operator()(int i) {
//		cout << i << endl;
		return i % 2 == 0;
	}
};

struct __factorial_function__ {
	int n;
	__factorial_function__() : n(1) { }
	void operator()(int i) {
		if (i != 0)
			n *= i;
	}
};

class Test_stream : public utils::test::TestCase {
public:

	Test_stream() {
		ADD_TEST_FUNCTION(Test_stream, Test_stream::test_filter);
//		ADD_TEST_FUNCTION(Test_stream, Test_stream::test_filter_parallel);
		ADD_TEST_FUNCTION(Test_stream, Test_stream::test_filter_foreach);
#if __cplusplus >= 201103L
		ADD_TEST_FUNCTION(Test_stream, Test_stream::test_filter_foreach2);
#endif
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_filter() {
		std::vector<int> vec;
		for (int i = 0; i < 1000; i++)
			vec.push_back(i);
		__pair_filter__ filter;
		std::vector<int> res = stream_of(vec).filter(filter).collection();
		ASSERT(res.size() == 500, "Expected 500 elements");
	}

	void test_filter_parallel() {
		std::vector<int> vec;
		for (int i = 0; i < 1000; i++)
			vec.push_back(i);
		__pair_filter__ filter;
		std::vector<int> res = stream_of(vec,free_thread_parallel_strategy(2)).filter(filter).collection();
		ASSERT(res.size() == 500, "Expected 500 elements");
	}

	void test_filter_foreach() {
		std::vector<int> vec;
		for (int i = 0; i <= 10; i++)
			vec.push_back(i);
		__factorial_function__ fact;
		stream_of(vec).forEach(&fact);
		ASSERT(fact.n == 3628800, "Expected 3628800 elements");
	}

#if __cplusplus >= 201103L

	void test_filter_foreach2() {
		std::vector<int> vec;
		for (int i = 0; i <= 10; i++)
			vec.push_back(i);
		int n = 1;
		stream_of(vec).forEach([&n](int i)->void { if (i != 0) n *= i; });
		cout << n << endl;
		ASSERT(n == 3628800, "Expected 3628800 elements");
	}

#endif

};


void register_test_containers() {
//	ADD_TEST_CASE(Test_array);
//	ADD_TEST_CASE(Test_hash_map);
//	ADD_TEST_CASE(Test_fwdlist);
	ADD_TEST_CASE(Test_stream);
}

