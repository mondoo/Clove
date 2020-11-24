#pragma once

#include "Clove/Maths/MathsTypes.hpp"

#include <glm/ext/scalar_constants.hpp>

namespace garlic::clove {
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
    mat<4, 4, T> constexpr createOrthographicMatrix(T left, T right, T bottom, T top);

    template<typename T>
    mat<4, 4, T> constexpr createOrthographicMatrix(T left, T right, T bottom, T top, T near, T far);

    template<typename T>
    mat<4, 4, T> constexpr createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar);

    template<typename T>
    quat<T> eulerToQuaternion(vec<3, T> const &euler);

    template<typename T>
    vec<3, T> matrixToEuler(mat<4, 4, T> const &mat);

    template<typename T>
    quat<T> matrixToQuaternion(mat<3, 3, T> const &mat);

    template<typename T>
    quat<T> matrixToQuaternion(mat<4, 4, T> const &mat);

    template<typename T>
    quat<T> asQuaternion(T angle, vec<3, T> const &axis);

    template<typename T>
    vec<3, T> quaternionToEuler(quat<T> const &quat);

    template<typename T>
    mat<3, 3, T> quaternionToMatrix3(quat<T> const &quat);

    template<typename T>
    mat<4, 4, T> quaternionToMatrix4(quat<T> const &quat);

    template<typename T>
    vec<3, T> screenToWorld(vec<2, T> const &screenPos, T screenDepth, vec<2, T> const &screenSize, mat<4, 4, T> const &viewMatrix, mat<4, 4, T> const &projectionMatrix);
}

#include "MathsHelpers.inl"