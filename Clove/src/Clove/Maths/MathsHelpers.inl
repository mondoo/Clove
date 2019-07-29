#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace clv::math{
	template<typename T>
	const typename T::value_type* valuePtr(const T& v){
		return glm::value_ptr(v);
	}

	template<typename T>
	typename T::value_type* valuePtr(T& v){
		return glm::value_ptr(v);
	}

	template<typename T>
	constexpr T asRadians(T degrees){
		return glm::radians(degrees);
	}

	template<typename T>
	constexpr T asDegrees(T radians){
		return glm::degrees(radians);
	}

	template<typename T>
	Matrix<4, 4, T, qualifier::defaultp> createOrthographicMatrix(T left, T right, T bottom, T top){
		return glm::ortho(left, right, bottom, top);
	}

	template<typename T>
	Matrix<4, 4, T, qualifier::defaultp> createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar){
		return glm::perspective(fovy, aspect, zNear, zFar);
	}

	template<typename T, qualifier Q>
	Quaternion<T, Q> asQuaternion(T angle, const Vector<3, T, Q>& axis){
		return glm::angleAxis(angle, axis);
	}

	template<typename T, qualifier Q>
	Matrix<3, 3, T, Q> quaternionToMatrix3(const Quaternion<T, Q>& quat){
		return glm::toMat3(quat);
	}

	template<typename T, qualifier Q>
	Matrix<4, 4, T, Q> quaternionToMatrix4(const Quaternion<T, Q>& quat){
		return glm::toMat4(quat);
	}

	template<typename T, qualifier Q>
	Quaternion<T, Q> matrixToQuaternion(const Matrix<3, 3, T, Q>& mat){
		return glm::toQuat(mat);
	}

	template<typename T, qualifier Q>
	Quaternion<T, Q> matrixToQuaternion(const Matrix<4, 4, T, Q>& mat){
		return glm::toQuat(mat);
	}

	template<typename T, qualifier Q>
	Vector<3, T, Q> quaternionToEuler(const Quaternion<T, Q>& quat){
		return glm::eulerAngles(quat);
	}
}