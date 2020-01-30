#pragma once

#include "Clove/Core/Maths/glmWrappers.hpp"

namespace clv::mth{
	inline float cos(float value);

	template<length_t L, typename T, qualifier Q>
	vec<L, T, Q> normalise(const vec<L, T, Q>& v);

	template<typename T, qualifier Q>
	vec<3, T, Q> cross(const vec<3, T, Q>& v1, const vec<3, T, Q>& v2);

	template<length_t L, typename T, qualifier Q>
	vec<L, T, Q> dot(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2);

	template<length_t L, typename T, qualifier Q>
	T distance(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2);

	template<length_t L, typename T, qualifier Q>
	T distanceSquared(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2);

	template<length_t L, typename T, qualifier Q>
	T length(const vec<L, T, Q>& vec);

	template<length_t L, typename T, qualifier Q>
	T lengthSquared(const vec<L, T, Q>& vec);

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> lookAt(const vec<3, T, Q>& eye, const vec<3, T, Q>& center, const vec<3, T, Q>& up);

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> translate(const mat<4, 4, T, Q>& m, const vec<3, T, Q>& v);

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> rotate(const mat<4, 4, T, Q>& m, T angle, const vec<3, T, Q>& v);

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> scale(const mat<4, 4, T, Q>& m, const vec<3, T, Q>& v);

	template<length_t C, length_t R, typename T, qualifier Q>
	mat<C, R, T, Q> inverse(const mat<C, R, T, Q>& m);

	template<length_t C, length_t R, typename T, qualifier Q>
	mat<C, R, T, Q> transpose(const mat<C, R, T, Q>& m);
}

#include "Maths.inl"