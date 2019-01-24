/*
 * any_value.hpp
 *
 *  Created on: 12 de jan de 2019
 *      Author: alanl
 */

#ifndef ANY_VALUE_HPP_
#define ANY_VALUE_HPP_

#include <type_traits>

#include <compys/compy_defs.hpp>

namespace compys {

union any_value_data {
	int i;
	float f;
	__int64 i64;
	double d;
	void* ptr;
	unsigned int u;
	unsigned __int64 u64;
	char* str;
	const char* cstr;
	any_value_data() :
			i64(0) {
	}
	any_value_data(int _i) :
			i(_i) {
	}
	any_value_data(float _f) :
			f(_f) {
	}
	any_value_data(__int64 _i64) :
			i64(_i64) {
	}
	any_value_data(double _d) :
			d(_d) {
	}
	any_value_data(void* _ptr) :
			ptr(_ptr) {
	}
	any_value_data(char* _str) :
			str(_str) {
	}
	any_value_data(const char* _str) :
			cstr(_str) {
	}
	any_value_data(unsigned int _ui) :
			u(_ui) {
	}
	any_value_data(unsigned __int64 _ui64) :
			u64(_ui64) {
	}
	template <class C>
	any_value_data(C* _ptr) :
			ptr((void*)_ptr) {
	}
};

struct any_data_struct {
	any_data_struct() { }
	virtual ~any_data_struct() = default;
	/**
	 * Overrides the default new operator
	 */
	static void* operator new(size_t sz);

	/**
	 * Overrides the default delete operator
	 */
	static void operator delete(void* p, size_t sz);

};

template <class T>
struct any_data_struct_impl : public any_data_struct {
	T value;
	template <class E>
	any_data_struct_impl(E _value) : value(_value) { }
	virtual ~any_data_struct_impl() = default;
};

struct any_value {
private:
	any_value_data _data;
	enum {
		T_VOID       = 0,
		T_INT        = 1,
		T_FLOAT      = 2,
		T_INT64      = 3,
		T_DOUBLE     = 4,
		T_PTR        = 5,
		T_CSTR       = 6,
		T_UINT       = 7,
		T_UINT64     = 8,
		T_ANY        = 9,
		T_REFERENCE  = 10,

		_FORCE_DWORD = 0xFFFFFFFF
	} _data_type;
	template <class T>
	typename std::enable_if<std::is_pointer<T>::value>::type
		assign_ptror(T obj) {
		_data.ptr = (void*) obj;
		_data_type = T_PTR;
	}

	template <class T>
	typename std::enable_if<!std::is_pointer<T>::value>::type
		assign_ptror(T obj) {
		assign_referenceor(obj);
	}

	template <class T>
	typename std::enable_if<std::is_reference<T>::value>::type
		assign_referenceor(T obj) {
		_data.ptr = (void*) &obj;
		_data_type = T_REFERENCE;
	}

	template <class T>
	typename std::enable_if<!std::is_reference<T>::value>::type
		assign_referenceor(T obj) {
		_data.ptr = (void*) new any_data_struct_impl<T>(obj);
		_data_type = T_ANY;
	}

	void delete_any_data() {
		if (_data_type == T_ANY) {
			any_data_struct* ds = reinterpret_cast<any_data_struct*>(_data.ptr);
			delete ds;
		}
	}
public:
	any_value() :
		_data(), _data_type(T_VOID) {
	}
	any_value(int _i) :
		_data(_i), _data_type(T_INT) {
	}
	any_value(float _f) :
		_data(_f), _data_type(T_FLOAT) {
	}
	any_value(__int64 _i64) :
		_data(_i64), _data_type(T_INT64) {
	}
	any_value(double _d) :
		_data(_d), _data_type(T_DOUBLE) {
	}
	any_value(void* _ptr) :
		_data(_ptr), _data_type(T_PTR) {
	}
	any_value(char* _str) :
		_data(_str), _data_type(T_CSTR) {
	}
	any_value(const char* _str) :
		_data(_str), _data_type(T_CSTR) {
	}
	any_value(unsigned int _ui) :
		_data(_ui), _data_type(T_UINT) {
	}
	any_value(unsigned __int64 _ui64) :
		_data(_ui64), _data_type(T_UINT64) {
	}
	template <class C>
	any_value(C* _ptr) :
		_data((void*)_ptr), _data_type(T_PTR)  {
	}
	template <class T>
	any_value(T obj) :
		_data(), _data_type(T_VOID)  {
		assign_ptror<T>(obj);
	}

	~any_value() {
		delete_any_data();
	}

	any_value& operator = (const any_value& other) {
		delete_any_data();
		_data.i64 = other._data.i64;
		return *this;
	}
	any_value& operator = (int _i) {
		delete_any_data();
		_data.i = _i;
		return *this;
	}
	any_value& operator = (float _f) {
		delete_any_data();
		_data.f = _f;
		return *this;
	}
	any_value& operator = (__int64 _i64) {
		delete_any_data();
		_data.i64 = _i64;
		return *this;
	}
	any_value& operator = (double _d) {
		delete_any_data();
		_data.d = _d;
		return *this;
	}
	any_value& operator = (unsigned int _i) {
		delete_any_data();
		_data.u = _i;
		return *this;
	}
	any_value& operator = (unsigned __int64 _i64) {
		delete_any_data();
		_data.u64 = _i64;
		return *this;
	}
	template<class C>
	any_value& operator = (C* _ptr) {
		delete_any_data();
		_data.ptr = (void*)(_ptr);
		return *this;
	}

	template<class C>
	any_value& operator = (C obj) {
		delete_any_data();
		assign_ptror<C>(obj);
		return *this;
	}

	template<class R>
	R conv() const {
		return reinterpret_cast<R>(_data.ptr);
	}

	template<class R>
	typename std::enable_if<std::is_class<R>::value,R>::type
		conv_struct() const {
		any_data_struct_impl<R>* data = reinterpret_cast<any_data_struct_impl<R>*>(_data.ptr);
		return 	data->value;
	}

};

template<>
inline int any_value::conv<int>() const {
	return _data.i;
}

template<>
inline unsigned int any_value::conv<unsigned int>() const {
	return _data.u;
}

template<>
inline float any_value::conv<float>() const {
	return _data.f;
}

template<>
inline __int64 any_value::conv<__int64>() const {
	return _data.i64;
}

template<>
inline unsigned __int64 any_value::conv<unsigned __int64>() const {
	return _data.u64;
}

template<>
inline double any_value::conv<double>() const {
	return _data.d;
}

template<>
inline void any_value::conv<void>() const {
}

} // namespace compys

#endif /* ANY_VALUE_HPP_ */
