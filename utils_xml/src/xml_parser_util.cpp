/*
 * xml_parser_util.cpp
 *
 *  Created on: 28 de jun de 2018
 *      Author: alan.lohse
 */

#include "xml_parser_util.h"

const char* _char_xml_constants[] = {
		"=", "\n", "<", ">", "/", "\"", ":", "&",
		";", "<![CDATA[", "]]>", "<!DOCTYPE", ">", "<!--", "-->", "<?xml", "?>",
		"version", "encoding", "standalone", "<!ELEMENT", "#PCDATA", "CDATA",
		"EMPTY", "<!ATTLIST", "<!ENTITY", "#REQUIRED", "#IMPLIED", "(", ")",
		"|", "+", "?", "#FIXED", "*", "ANY", "[", "]", "/>"
};

const wchar_t* _wchar_xml_constants[] = {
		L"=", L"\n", L"<", L">", L"/", L"\"",
		L":", L"&", L";", L"<![CDATA[", L"]]>", L"<!DOCTYPE", L">", L"<!--",
		L"-->", L"<?xml", L"?>", L"version", L"encoding", L"standalone",
		L"<!ELEMENT", L"#PCDATA", L"CDATA", L"EMPTY", L"<!ATTLIST", L"<!ENTITY",
		L"#REQUIRED", L"#IMPLIED", L"(", L")", L"|", L"+", L"?", L"#FIXED",
		L"*", L"ANY", L"[", L"]", L"/>"
};

std::regex IS_TAG("^<([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)(\\s.*?)?/?>$", std::regex_constants::icase);
std::wregex IS_TAGW(L"^<([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)(\\s.*?)?/?>$", std::regex_constants::icase);
std::regex CLOSE_TAG("^</([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)>$", std::regex_constants::icase);
std::wregex CLOSE_TAGW(L"^</([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)>$", std::regex_constants::icase);
std::regex ATTRS("^\\s*([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)=\"([^\"]*)\"\\s*", std::regex_constants::icase);
std::wregex ATTRSW(L"^\\s*([a-z][a-z0-9-_]+[:])?([a-z][a-z0-9-_]+)=\"([^\"]*)\"\\s*", std::regex_constants::icase);
std::regex ENT_NUM("^#\\d+$");
std::regex ENT_HEX("^#x[0-9a-fA-F]+$");
std::wregex ENT_NUMW(L"^#\\d+$");
std::wregex ENT_HEXW(L"^#x[0-9a-fA-F]+$");

