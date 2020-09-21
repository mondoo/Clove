#pragma once

namespace clv::mth {
    template<typename T>
    const typename T::value_type* valuePtr(const T& v);

    template<typename T>
    typename T::value_type* valuePtr(T& v);

    template<typename T>
    constexpr T asRadians(T degrees);

    template<typename T>
    constexpr T asDegrees(T radians);

    template<typename T>
    constexpr T pi = glm::pi<T>();

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> createOrthographicMatrix(T left, T right, T bottom, T top);

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> createOrthographicMatrix(T left, T right, T bottom, T top, T near, T far);

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar);

    template<typename T, qualifier Q>
    quat<T, Q> eulerToQuaternion(const vec<3, T, Q>& euler);

    template<typename T, qualifier Q>
    vec<3, T, Q> matrixToEuler(const mat<4, 4, T, Q>& mat);

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(const mat<3, 3, T, Q>& mat);

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(const mat<4, 4, T, Q>& mat);

    template<typename T, qualifier Q>
    quat<T, Q> asQuaternion(T angle, const vec<3, T, Q>& axis);

    template<typename T, qualifier Q>
    vec<3, T, Q> quaternionToEuler(const quat<T, Q>& quat);

    template<typename T, qualifier Q>
    mat<3, 3, T, Q> quaternionToMatrix3(const quat<T, Q>& quat);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> quaternionToMatrix4(const quat<T, Q>& quat);

    template<typename T, qualifier Q>
    vec<3, T, Q> screenToWorld(const vec<2, T, Q>& screenPos, T screenDepth, const vec<2, T, Q>& screenSize, const mat<4, 4, T, Q>& viewMatrix, const mat<4, 4, T, Q>& projectionMatrix);
}

#include "MathsHelpers.inl"