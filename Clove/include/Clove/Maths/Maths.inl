#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

namespace clv::mth{
	template<length_t L, typename T, qualifier Q>
	vec<L, T, Q> normalise(const vec<L, T, Q>& v){
		return glm::normalize(v);
	}

	template<typename T, qualifier Q>
	vec<3, T, Q> cross(const vec<3, T, Q>& v1, const vec<3, T, Q>& v2){
		return glm::cross(v1, v2);
	}

	template<length_t L, typename T, qualifier Q>
	vec<L, T, Q> dot(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2){
		return glm::dot(v1, v2);
	}

	template<length_t L, typename T, qualifier Q>
	T distance(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2){
		return glm::distance(v1, v2);
	}

	template<length_t L, typename T, qualifier Q>
	T distanceSquared(const vec<L, T, Q>& v1, const vec<L, T, Q>& v2){
		return glm::distance2(v1, v2);
	}

	template<length_t L, typename T, qualifier Q>
	T length(const vec<L, T, Q>& vec){
		return glm::length(vec);
	}

	template<length_t L, typename T, qualifier Q>
	T lengthSquared(const vec<L, T, Q>& vec){
		return glm::length2(vec);
	}

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> lookAt(const vec<3, T, Q>& eye, const vec<3, T, Q>& center, const vec<3, T, Q>& up){
		return glm::lookAt(eye, center, up);
	}

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> translate(const mat<4, 4, T, Q>& m, const vec<3, T, Q>& v){
		return glm::translate(m, v);
	}

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> rotate(const mat<4, 4, T, Q>& m, T angle, const vec<3, T, Q>& v){
		return glm::rotate(m, angle, v);
	}

	template<typename T, qualifier Q>
	quat<T, Q> rotate(T angle, const vec<3, T, Q>& v) {
		return {
			cos(angle / 2.0f),
			sin((angle * v.x) / 2.0f),
			sin((angle * v.y) / 2.0f),
			sin((angle * v.z) / 2.0f)
		};
	}

	template<typename T, qualifier Q>
	mat<4, 4, T, Q> scale(const mat<4, 4, T, Q>& m, const vec<3, T, Q>& v){
		return glm::scale(m, v);
	}

	template<length_t C, length_t R, typename T, qualifier Q>
	mat<C, R, T, Q> inverse(const mat<C, R, T, Q>& m){
		return glm::inverse(m);
	}

	template<length_t C, length_t R, typename T, qualifier Q>
	mat<C, R, T, Q> transpose(const mat<C, R, T, Q>& m){
		return glm::transpose(m);
	}

	template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> lerp(const vec<L, T, Q>& a, const vec<L, T, Q>& b, T t) {
        return glm::mix(a, b, t);
    }

	template<typename T, qualifier Q>
    quat<T, Q> lerp(const quat<T, Q>& a, const quat<T, Q>& b, T t) {
        return glm::lerp(a, b, t);
    }

	template<typename T, qualifier Q>
    quat<T, Q> slerp(const quat<T, Q>& a, const quat<T, Q>& b, T t) {
        return glm::slerp(a, b, t);
    }
}