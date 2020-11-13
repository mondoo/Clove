#pragma once

namespace clv::mth {
    template<typename T>
    typename T::value_type const *valuePtr(T const &v);

    template<typename T>
    typename T::value_type *valuePtr(T &v);

    template<typename T>
    T constexpr asRadians(T degrees);

    template<typename T>
    T constexpr asDegrees(T radians);

    template<typename T>
    T constexpr pi = glm::pi<T>();

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createOrthographicMatrix(T left, T right, T bottom, T top);

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createOrthographicMatrix(T left, T right, T bottom, T top, T near, T far);

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar);

    template<typename T, qualifier Q>
    quat<T, Q> eulerToQuaternion(vec<3, T, Q> const &euler);

    template<typename T, qualifier Q>
    vec<3, T, Q> matrixToEuler(mat<4, 4, T, Q> const &mat);

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(mat<3, 3, T, Q> const &mat);

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(mat<4, 4, T, Q> const &mat);

    template<typename T, qualifier Q>
    quat<T, Q> asQuaternion(T angle, vec<3, T, Q> const &axis);

    template<typename T, qualifier Q>
    vec<3, T, Q> quaternionToEuler(quat<T, Q> const &quat);

    template<typename T, qualifier Q>
    mat<3, 3, T, Q> quaternionToMatrix3(quat<T, Q> const &quat);

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> quaternionToMatrix4(quat<T, Q> const &quat);

    template<typename T, qualifier Q>
    vec<3, T, Q> screenToWorld(vec<2, T, Q> const &screenPos, T screenDepth, vec<2, T, Q> const &screenSize, mat<4, 4, T, Q> const &viewMatrix, mat<4, 4, T, Q> const &projectionMatrix);
}

#include "MathsHelpers.inl"