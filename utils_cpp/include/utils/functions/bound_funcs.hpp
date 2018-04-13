/*
 * bound_funcs.hpp
 *
 *  Created on: 30/03/2010
 *      Author: alan.lohse
 */

#ifndef BOUND_FUNCS_HPP_
#define BOUND_FUNCS_HPP_

namespace utils {

namespace functions {

class bound_func {
public:
	virtual ~bound_func(){}
	virtual void call(void* ret, void* obj, void* params[]) = 0;
};

template<typename _Rt, typename _Cl>
class bound_func_NP : public bound_func {
public:
	typedef void (_Cl::*func_type)();
	func_type func;
	bound_func_NP(func_type _func) {
		func = _func;
	}
	virtual void call(void* ret, void* obj, void* params[]) {
		*((_Rt*)ret) = (((_Cl*)obj)->*func)();
	}
};

template<typename _Cl>
class bound_func_NP<void,_Cl> : public bound_func {
public:
	typedef void (_Cl::*func_type)();
	func_type func;
	bound_func_NP(func_type _func) {
		func = _func;
	}
	virtual void call(void* ret, void* obj, void* params[]) {
		(((_Cl*)obj)->*func)();
	}
};

template<typename _Rt, typename _Cl, typename _P1>
class bound_func_1P : public bound_func {
public:
	typedef void (_Cl::*func_type)(_P1);
	func_type func;
	bound_func_1P(func_type _func) {
		func = _func;
	}
	virtual void call(void* ret, void* obj, void* params[]) {
		*((_Rt*)ret) = (((_Cl*)obj)->*func)(*(_P1*)params[0]);
	}
};

template<typename _Cl, typename _P1>
class bound_func_1P<void,_Cl,_P1> : public bound_func {
public:
	typedef void (_Cl::*func_type)(_P1);
	func_type func;
	bound_func_1P(func_type _func) {
		func = _func;
	}
	virtual void call(void* ret, void* obj, void* params[]) {
		(((_Cl*)obj)->*func)(*(_P1*)params[0]);
	}
};

}

}

#endif /* BOUND_FUNCS_HPP_ */
