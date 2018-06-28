/*
 * test_main.cpp
 *
 *  Created on: 15 de jun de 2018
 *      Author: alan.lohse
 */

#include <utils/xml/xml_parser.hpp>
#include <utils/test/test_tools.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace utils;
using namespace utils::xml;


char xml1[] = R"XML1DEM(<?xml version="1.0" encoding="UTF-8" ?>
<main>
	<!--a comment here-->
	<child-element atribute1="value of attribute1">
		Some text &amp; entities
	</child-element>
	<a-leaf-tag/>
	<a-leaf-tag with="attributes" />
	<![CDATA[
A cdata text with prohibited characters like > and < or &
	]]>
</main>
)XML1DEM";

class Xml1Handler : public tag_handler<char> {
public:

	Xml1Handler(){}
	virtual ~Xml1Handler(){}

	virtual void start_document(){}

	virtual void end_document(){}

	virtual void start_element(const char_type* tag, const char_type* prefix, const std::vector<attribute<char_type> >& attributes) {
		cout << "Start element " << prefix << ":" << tag << endl;
		cout << "Attributes ";
		std::vector<attribute<char_type> >::const_iterator it = attributes.begin();
		std::vector<attribute<char_type> >::const_iterator end = attributes.end();
		for (; it != end; ++it) {
			cout << it->prefix << ":" << it->name << "=" << it->value << endl;
		}
	}

	virtual void end_element(const char_type* tag, const char_type* prefix) {
		cout << "End element " << prefix << ":" << tag << endl;
	}

	virtual void xml_decl(const char_type* version, const char_type* encoding, const char_type* standalone) {
		cout << "Xml:";
		if (version)
			cout << " version=" << version;
		if (encoding)
			cout << " encoding=" << encoding;
		if (standalone)
			cout << " standalone=" << standalone;
		cout << endl;
	}

	virtual void characters (const char_type* begin, const char_type* end) {
		cout << "Characters " << begin << endl;
	}

	virtual void comments (const char_type* begin, const char_type* end) {
		cout << "Comments '" << begin << "'"<< endl;
	}

	virtual const char_type* entity (const char_type* name) {
		return "";
	}

	virtual void warning (const xml_parser_exception& e) {}

	virtual void error (const xml_parser_exception& e) {
		cout << "Error: " << e.what() << endl;
	}

	virtual void fatal_error (const xml_parser_exception& e) {
		cout << "Fatal error: " << e.what() << endl;
	}

};

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

class Test_Xml_parser : public utils::test::TestCase {
public:

	Test_Xml_parser() {
		ADD_TEST_FUNCTION(Test_Xml_parser, Test_Xml_parser::test_random);
	}

	virtual void prepare_test() { }
	virtual void close_test() { }

	void test_random() {

		utils::xml::xml_parser parser;
		membuf sbuf(xml1, xml1 + strlen(xml1));
		cout << xml1 << endl;
		std::istream is(&sbuf);
		Xml1Handler handler;
		parser.parse(is, handler);

	}

};


int main() {
	ADD_TEST_CASE(Test_Xml_parser);

	cout << "Starting tests" << endl;
	return utils::test::TestSuit::run_tests();

}

