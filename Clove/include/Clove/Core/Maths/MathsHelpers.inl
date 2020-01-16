#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace clv::mth{
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
	mat<4, 4, T, qualifier::defaultp> createOrthographicMatrix(T left, T right, T bottom, T top){
		return glm::ortho(left, right, bottom, top);
	}

	template<typename T>
	mat<4, 4, T, qualifier::defaultp> createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar){
		return glm::perspective(fovy, aspect, zNear, zFar);
	}

	template<typename T, qualifier Q>
	quat<T, Q> eulerToQuaternion(const vec<3, T, Q>& euler){
		return matrixToQuaternion(glm::yawPitchRoll(euler.y, euler.x, euler.z));
	}

	template<typename T, qualifier Q>
	vec<3, T, Q> matrixToEuler(const mat<4, 4, T, Q>& mat){
		vec<3, T, Q> vec;
		glm::extractEulerAngleXYZ(mat, vec.x, vec.y, vec.z);
		return vec;
	}

	template<typename T, qualifier Q>
	quat<T, Q> matrixToQuaternion(const mat<3, 3, T, Q>& mat){
		return glm::toQuat(mat);
	}

	template<typename T, qualifier Q>
	quat<T, Q> matrixToQuaternion(const mat<4, 4, T, Q>& mat){
		return glm::toQuat(mat);
	}

	template<typename T, qualifier Q>
	quat<T, Q> asQuaternion(T angle, const vec<3, T, Q>& axis){
		return glm::angleAxis(angle, axis);
	}

	template<typename T, qualifier Q>
	vec<3, T, Q> quaternionToEuler(const quat<T, Q>& quat){
		return (glm::axis(quat) * glm::angle(quat));
	}

	template<typename T, qualifier Q>
	mat<3, 3, T, Q> quaternionToMatrix3(const quat<T, Q>& quat){
		return glm::toMat3(quat);
	}

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> quaternionToMatrix4(const quat<T, Q>& quat){
		return glm::toMat4(quat);
	}
}