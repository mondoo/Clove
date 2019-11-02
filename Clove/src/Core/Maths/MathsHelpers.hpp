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

	template<typename T, qualifier Q>
	Quaternion<T, Q> asQuaternion(T angle, const Vector<3, T, Q>& axis);

	template<typename T, qualifier Q>
	Matrix<3, 3, T, Q> quaternionToMatrix3(const Quaternion<T, Q>& quat);

	template<typename T, qualifier Q>
	Matrix<4, 4, T, Q> quaternionToMatrix4(const Quaternion<T, Q>& quat);

	template<typename T, qualifier Q>
	Quaternion<T, Q> matrixToQuaternion(const Matrix<3, 3, T, Q>& mat);

	template<typename T, qualifier Q>
	Quaternion<T, Q> matrixToQuaternion(const Matrix<4, 4, T, Q>& mat);

	template<typename T, qualifier Q>
	Vector<3, T, Q> quaternionToEuler(const Quaternion<T, Q>& quat);

	template<typename T, qualifier Q>
	Vector<3, T, Q> eulerFromMatrix(const Matrix<4, 4, T, Q>& mat);
}

#include "MathsHelpers.inl"