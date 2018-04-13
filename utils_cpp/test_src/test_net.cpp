/*
 * test_net.cpp
 *
 *  Created on: 13/04/2011
 *      Author: Alan
 */

#include <utils/time.hpp>
#include <utils/test/test_tools.hpp>
#include <utils/net/url.hpp>
#include <utils/net/socket_iostream.hpp>
#include <utils/net/socket.hpp>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
using namespace utils::net;

class Test_url : public utils::test::TestCase {
public:

	Test_url() {
		ADD_TEST_FUNCTION(Test_url, Test_url::test_url);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_url() {
		url u = url("HTTP://www.utilscpp.org:8888/test/net;id=123676?arg1=1;arg2=atext;arg3=a+%2B+b+%3Dc");
		ASSERT2(u.get_protocol(),"http");
		ASSERT2(u.get_host(),"www.utilscpp.org");
		ASSERT2(u.get_port(),8888);
		ASSERT2(u.get_path(),"/test/net;id=123676");
		ASSERT2(u.get_query(),"arg1=1;arg2=atext;arg3=a+%2B+b+%3Dc");
	}

};

class Test_socket : public utils::test::TestCase {
public:

	Test_socket() {
		ADD_TEST_FUNCTION(Test_socket, Test_socket::test_socket);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_socket() {
//		socket client;
//		socket server;
//		socket_address addr("localhost",7777);
//		server.bind(addr);
//		server.listen(1);
//		socket newsock = server.accept();
	}

};

void register_test_net() {
	ADD_TEST_CASE(Test_url);
	ADD_TEST_CASE(Test_socket);
}

