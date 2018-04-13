/*
 * statictics.hpp
 *
 *  Created on: 03/02/2014
 *      Author: Alan
 */

#ifndef STATICTICS_HPP_
#define STATICTICS_HPP_

#include <utils/utils_defs.hpp>
#include <utils/funcs.hpp>
#include <utils/math/math.hpp>
#include <utils/math/math_exception.hpp>
#include <vector>

namespace utils {

namespace math {

template<typename _Float, typename _Alloc = DEFAULT_ALLOCATOR<_Float > >
class statistics {
public:
	typedef _Float float_type;
	typedef std::vector<float_type,_Alloc> vector_type;
	typedef typename vector_type::iterator iterator;
	typedef typename vector_type::const_iterator const_iterator;
	typedef typename std::vector<float_type,_Alloc>::size_type size_type;
private:
	vector_type M_data;
	bool M_modified;
	float_type M_sum;
	float_type M_variance;
	float_type M_max;
	float_type M_min;
	void recalc() {
		if (M_data.size() == 1) throw math_exception("Must have more samples.");
		typename vector_type::iterator it = M_data.begin();
		typename vector_type::iterator end = M_data.end();
		M_variance = 0;
		float_type M_mean = mean();
		for (; it < end; ++it) {
			M_variance += Math::sqr(*it - M_mean);
		}
		M_modified = false;
	}
public:
	statistics() : M_data(), M_modified(false),
			M_sum(0),
			M_variance(0) {
	}

	~statistics() {
	}

	void push(const float_type& v) {
		M_data.push_back(v);
		M_modified = true;
		M_sum += v;
	}

	float_type mean() const {
		return M_sum/M_data.size();
	}

	float_type sum() const {
		return M_sum;
	}

	float_type max() const {
		return M_max;
	}

	float_type min() const {
		return M_min;
	}

	float_type standard_deviation() {
		return Math::sqrt(variance());
	}

	float_type standard_sample_deviation() {
		return Math::sqrt(sample_variance());
	}

	float_type variance() {
		if (M_modified) {
			recalc();
		}
		return M_variance/ Math::sqr(M_data.size());
	}

	float_type sample_variance() {
		if (M_modified) {
			recalc();
		}
		return M_variance/Math::sqr(M_data.size()-1);
	}

	const float_type& sample(size_type index) {
		return M_data.at(index);
	}

	size_type count() const {
		return M_data.size();
	}

	iterator begin_samples() {
		return M_data.begin();
	}

	iterator end_samples() {
		return M_data.end();
	}

	const_iterator begin_samples() const {
		return M_data.begin();
	}

	const_iterator end_samples() const {
		return M_data.end();
	}

};

template<typename _Float>
class error_measure {
public:
	typedef _Float float_type;
	typedef size_t size_type;
private:
	float_type _sqr_error;
	size_type _count;

public:
	error_measure() : _sqr_error(0), _count(0) { }

	void push(float_type value, float_type target) {
		_sqr_error = Math::sqr(target - value);
		++_count;
	}

	template<class _Iterator>
	void push(_Iterator value_begin, _Iterator value_end, _Iterator target_begin, _Iterator target_end) {
		_Iterator itv = value_begin;
		_Iterator itt = target_begin;
		for (; itv < value_end && itt < target_end; ++itv, ++itt)
			push(*itv, *itt);
	}

	float_type rms() const {
		return Math::sqrt(_sqr_error / _count);
	}

	float_type sqr_sum() const {
		return _sqr_error;
	}

	size_type size() const {
		return _count;
	}

};

}

}

#endif /* STATICTICS_HPP_ */
