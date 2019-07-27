#pragma once

namespace clv::math{
	template<typename T>
	const typename T::value_type* valuePtr(const T& v);

	template<typename T>
	typename T::value_type* valuePtr(T& v);

	template<typename T>
	constexpr T asRadians(T degrees);

	template<typename T>
	constexpr T asDegrees(T radians);

	template<typename T>
	Matrix<4, 4, T, qualifier::defaultp> createOrthographicMatrix(T left, T right, T bottom, T top);

	template<typename T>
	Matrix<4, 4, T, qualifier::defaultp> createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar);
}

#include "MathsHelpers.inl"