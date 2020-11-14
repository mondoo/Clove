#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace clv::mth {
    template<typename T>
    typename T::value_type const *valuePtr(T const &v) {
        return glm::value_ptr(v);
    }

    template<typename T>
    typename T::value_type *valuePtr(T &v) {
        return glm::value_ptr(v);
    }

    template<typename T>
    T constexpr asRadians(T degrees) {
        return glm::radians(degrees);
    }

    template<typename T>
    T constexpr asDegrees(T radians) {
        return glm::degrees(radians);
    }

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createOrthographicMatrix(T left, T right, T bottom, T top) {
        //Swap the bottom and top so the winding order is correct after applying the matrix
        return glm::ortho(left, right, top, bottom);
    }

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createOrthographicMatrix(T left, T right, T bottom, T top, T near, T far) {
        //Swap the bottom and top so the winding order is correct after applying the matrix
        return glm::ortho(left, right, top, bottom, near, far);
    }

    template<typename T>
    mat<4, 4, T, qualifier::defaultp> constexpr createPerspectiveMatrix(T fovy, T aspect, T zNear, T zFar) {
        /*
            Defining our own implementation for this function as using glm with vulkan will
            result in y+ moving objects downwards.
        */

        T const tanHalfFovy{ tan(fovy / static_cast<T>(2)) };

        mat<4, 4, T, qualifier::defaultp> result{ static_cast<T>(0) };
        result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
        result[1][1] = -(static_cast<T>(1) / (tanHalfFovy));
        result[2][2] = zFar / (zFar - zNear);
        result[2][3] = static_cast<T>(1);
        result[3][2] = -(zFar * zNear) / (zFar - zNear);

        return result;
    }

    template<typename T, qualifier Q>
    quat<T, Q> eulerToQuaternion(vec<3, T, Q> const &euler) {
        return matrixToQuaternion(glm::yawPitchRoll(euler.y, euler.x, euler.z));
    }

    template<typename T, qualifier Q>
    vec<3, T, Q> matrixToEuler(mat<4, 4, T, Q> const &mat) {
        vec<3, T, Q> vec;
        glm::extractEulerAngleXYZ(mat, vec.x, vec.y, vec.z);
        return vec;
    }

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(mat<3, 3, T, Q> const &mat) {
        return glm::toQuat(mat);
    }

    template<typename T, qualifier Q>
    quat<T, Q> matrixToQuaternion(mat<4, 4, T, Q> const &mat) {
        return glm::toQuat(mat);
    }

    template<typename T, qualifier Q>
    quat<T, Q> asQuaternion(T angle, vec<3, T, Q> const &axis) {
        return glm::angleAxis(angle, axis);
    }

    template<typename T, qualifier Q>
    vec<3, T, Q> quaternionToEuler(quat<T, Q> const &quat) {
        return (glm::axis(quat) * glm::angle(quat));
    }

    template<typename T, qualifier Q>
    mat<3, 3, T, Q> quaternionToMatrix3(quat<T, Q> const &quat) {
        return glm::toMat3(quat);
    }

    template<typename T, qualifier Q>
    mat<4, 4, T, Q> quaternionToMatrix4(quat<T, Q> const &quat) {
        return glm::toMat4(quat);
    }

    template<typename T, qualifier Q>
    vec<3, T, Q> screenToWorld(vec<2, T, Q> const &screenPos, T screenDepth, vec<2, T, Q> const &screenSize, mat<4, 4, T, Q> const &viewMatrix, mat<4, 4, T, Q> const &projectionMatrix) {
        vec<4, T, Q> NDC{
            ((screenPos.x / screenSize.x) - 0.5f) * 2.0f,
            -((screenPos.y / screenSize.y) - 0.5f) * 2.0f,
            screenDepth,
            1.0f
        };

        mat<4, 4, T, Q> inverseProjView = mth::inverse(projectionMatrix * viewMatrix);

        vec<4, T, Q> world = inverseProjView * NDC;
        world /= world.w;

        return vec<3, T, Q>{ world.x, world.y, world.z };
    }
}