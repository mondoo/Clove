#pragma once

#include "Clove/Maths/glmWrappers.hpp"

namespace garlic::clove {
    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> normalise(vec<L, T, Q> const &v);

    template<typename T, qualifier Q>
    vec<3, T, Q> cross(vec<3, T, Q> const &v1, vec<3, T, Q> const &v2);

    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> dot(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2);

    template<length_t L, typename T, qualifier Q>
    T distance(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2);

    template<length_t L, typename T, qualifier Q>
    T distanceSquared(vec<L, T, Q> const &v1, vec<L, T, Q> const &v2);

    template<length_t L, typename T, qualifier Q>
    T length(vec<L, T, Q> const &vec);

    template<length_t L, typename T, qualifier Q>
    T lengthSquared(vec<L, T, Q> const &vec);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> lookAt(vec<3, T, Q> const &eye, vec<3, T, Q> const &center, vec<3, T, Q> const &worldUp);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> translate(mat<4, 4, T, Q> const &m, vec<3, T, Q> const &v);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> rotate(mat<4, 4, T, Q> const &m, T angle, vec<3, T, Q> const &v);

    template<typename T, qualifier Q>
    quat<T, Q> rotate(T angle, vec<3, T, Q> const &v);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> scale(mat<4, 4, T, Q> const &m, vec<3, T, Q> const &v);

    template<length_t C, length_t R, typename T, qualifier Q>
    mat<C, R, T, Q> inverse(mat<C, R, T, Q> const &m);

    template<length_t C, length_t R, typename T, qualifier Q>
    mat<C, R, T, Q> transpose(mat<C, R, T, Q> const &m);

    //Linear interpolate between two vectors
    template<length_t L, typename T, qualifier Q>
    vec<L, T, Q> lerp(vec<L, T, Q> const &a, vec<L, T, Q> const &b, T t);

    //Linear interpolate between two quaternions
    template<typename T, qualifier Q>
    quat<T, Q> lerp(quat<T, Q> const &a, quat<T, Q> const &b, T t);

    //Spherical linear interpolate between two quaternions
    template<typename T, qualifier Q>
    quat<T, Q> slerp(quat<T, Q> const &a, quat<T, Q> const &b, T t);
}

#include "Maths.inl"