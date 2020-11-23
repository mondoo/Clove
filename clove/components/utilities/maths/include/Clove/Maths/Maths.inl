#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

namespace garlic::clove {
    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> normalise(vec<L, T, Q> const &v) {
        return glm::normalize(v);
    }

    template<typename T, qualifier Q>
    vec<3, T, Q> cross(vec<3, T, Q> const &v1, vec<3, T, Q> const &v2) {
        return glm::cross(v1, v2);
    }

    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> dot(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2) {
        return glm::dot(v1, v2);
    }

    template<length_t L, typename T, qualifier Q>
    T distance(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2) {
        return glm::distance(v1, v2);
    }

    template<length_t L, typename T, qualifier Q>
    T distanceSquared(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2) {
        return glm::distance2(v1, v2);
    }

    template<length_t L, typename T, qualifier Q>
    T length(vec<L, T, Q> const &vec) {
        return glm::length(vec);
    }

    template<length_t L, typename T, qualifier Q>
    T lengthSquared(vec<L, T, Q> const &vec) {
        return glm::length2(vec);
    }

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> lookAt(vec<3, T, Q> const &eye, vec<3, T, Q> const &center, vec<3, T, Q> const &worldUp) {
        return glm::lookAt(eye, center, worldUp);
    }

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> translate(mat<4, 4, T, Q> const &m, vec<3, T, Q> const &v) {
        return glm::translate(m, v);
    }

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> rotate(mat<4, 4, T, Q> const &m, T angle, vec<3, T, Q> const &v) {
        return glm::rotate(m, angle, v);
    }

    template<typename T, qualifier Q>
    quat<T, Q> rotate(T angle, vec<3, T, Q> const &v) {
        return {
            cos(angle / 2.0f),
            sin((angle * v.x) / 2.0f),
            sin((angle * v.y) / 2.0f),
            sin((angle * v.z) / 2.0f)
        };
    }

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> scale(mat<4, 4, T, Q> const &m, vec<3, T, Q> const &v) {
        return glm::scale(m, v);
    }

    template<length_t C, length_t R, typename T, qualifier Q>
    mat<C, R, T, Q> inverse(mat<C, R, T, Q> const &m) {
        return glm::inverse(m);
    }

    template<length_t C, length_t R, typename T, qualifier Q>
    mat<C, R, T, Q> transpose(mat<C, R, T, Q> const &m) {
        return glm::transpose(m);
    }

    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> lerp(vec<L, T, Q> const &a, vec<L, T, Q> const &b, T t) {
        return glm::mix(a, b, t);
    }

    template<typename T, qualifier Q>
    quat<T, Q> lerp(quat<T, Q> const &a, quat<T, Q> const &b, T t) {
        return glm::lerp(a, b, t);
    }

    template<typename T, qualifier Q>
    quat<T, Q> slerp(quat<T, Q> const &a, quat<T, Q> const &b, T t) {
        return glm::slerp(a, b, t);
    }
}