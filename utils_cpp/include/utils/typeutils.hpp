/*
 * typeutils.hpp
 *
 *  Created on: 26/03/2010
 *      Author: alan.lohse
 */

#ifndef TYPEUTILS_HPP_
#define TYPEUTILS_HPP_

#include <utils/funcs.hpp>
#include <typeinfo>
#include <cstdlib>
#include <string>

namespace utils {

class typeutils {
private:
static std::string parse_name_GNU(const char** src) {
	std::string r;
	bool once = true;
	do {
		bool hasNS = false;
		int ptt = 0;
		bool isStd = false;
		if (!once) r += ", ";
		while (**src == 'P' || **src == 'N' || **src == 'S' || **src == 'K' || **src == 't') {
			switch(**src) {
			case 'N':
				hasNS = true;
			break;
			case 'S': if (!hasNS) {
				if (strncmp((*src),"S_",2)==0) {
				} else if (strncmp((*src),"S1_",3)==0) {
					(*src)++;
				} else if (strncmp((*src),"S2_",3)==0) {
					r+="const ";
					(*src)++;
				} else {
					isStd = true;
					r+="std::";
				}
			}
			break;
			case 'P': ptt++; break;
			case 'K': r+="const "; break;
			}
			(*src)++;
		}
		if (**src >= '0' && **src <= '9') {
			bool first = true;
			do {
				if (!first) r += "::";
				int sn = 0;
				for (const char* xx = *src; *xx >= '0' && *xx <= '9'; xx++) { sn++; }
				char tmp2[13] = "\0";
				strncpy(tmp2,*src,sn);
				int sz = atoi(tmp2);
				char temp[128];
				memset(temp,0,128);
				strncpy(temp,(*src)+sn,sz);
				(*src) += sn+sz;
				r += temp;
				first = false;
				if (**src == 'I') {
					(*src)++;
					r+='<';
					r+=parse_name_GNU(src);
					r+='>';
					if (**src != 0)(*src)++;
				}
			} while (**src >= '0' && **src <= '9' && **src != 0);
		} else {
			if (!isStd) {
				switch(**src) {
					case 'i': r+="int"; break;
					case 'c': r+="char"; break;
					case 'l': r+="long"; break;
					case 's': r+="short"; break;
					case 'x': r+="long long"; break;
					case 'm': r+="unsigned long"; break;
					case 'j': r+="unsigned int"; break;
					case 't': r+="unsigned short"; break;
					case 'h': r+="unsigned char"; break;
					case 'y': r+="unsigned long long"; break;
					case '_': r+="char*"; break;
				}
			} else {
				switch(**src) {
				case 's': r+="string"; break;
				case 'a': r+="allocator"; break;
				}
			}
			(*src)++;
			if (**src == 'I') {
				(*src)++;
				r+='<';
				r+=parse_name_GNU(src);
				r+='>';
				if (**src != 0)(*src)++;
			}
		}
		while (ptt--) r += '*';
		once = false;
	} while (**src != 'E' && **src != 0);
	if (**src=='E')
		(*src)++;
	return r;
}
public:
template<typename A>
static std::string type_name(A* a) {
	const char* src = typeid(*a).name();
#ifdef __GNUC__
	return parse_name_GNU(&src);
#else
	return src;
#endif
}

template<class A>
static std::string type_name(A& a) {
	return type_name(&a);
}

template<class A, class B>
static bool is_instance_of(B* a) {
	return dynamic_cast<A*> (a) != NULL;
}

template<class A, class B>
static bool is_instance_of(B& a) {
	return dynamic_cast<A> (a) != NULL;
}

};

}

#endif /* TYPEUTILS_HPP_ */
