#include <glm/gtc/type_ptr.hpp>

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
}