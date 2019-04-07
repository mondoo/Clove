#pragma once

#include "Clove/Maths/glmWrappers.h"

namespace clv{
	namespace math{
		template<length_t L, typename T, qualifier Q>
		Vector<L, T, Q> normalise(const Vector<L, T, Q>& v);

		template<typename T, qualifier Q>
		Vector<3, T, Q> cross(const Vector<3, T, Q>& v1, const Vector<3, T, Q>& v2);

		template<length_t L, typename T, qualifier Q>
		Vector<L, T, Q> dot(const Vector<L, T, Q>& v1, const Vector<L, T, Q>& v2);

		template<typename T, qualifier Q>
		Matrix<4, 4, T, Q> lookAt(const Vector<3, T, Q>& eye, const Vector<3, T, Q>& center, const Vector<3, T, Q>& up);
	
		template<typename T, qualifier Q>
		Matrix<4, 4, T, Q> translate(const Matrix<4, 4, T, Q>& m, const Vector<3, T, Q>& v);

		template<typename T, qualifier Q>
		Matrix<4, 4, T, Q> rotate(const Matrix<4, 4, T, Q>& m, T angle, const Vector<3, T, Q>& v);

		template<typename T, qualifier Q>
		Matrix<4, 4, T, Q> scale(const Matrix<4, 4, T, Q>& m, const Vector<3, T, Q>& v);
	}
}

#include "Maths.inl"