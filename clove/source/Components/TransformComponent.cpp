#include "Clove/Components/TransformComponent.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Maths/Maths.hpp>
#include <Clove/Maths/MathsHelpers.hpp>

namespace garlic::clove {
    TransformComponent::TransformComponent() = default;

    TransformComponent::TransformComponent(TransformComponent const &other) = default;

    TransformComponent::TransformComponent(TransformComponent &&other) noexcept = default;

    TransformComponent &TransformComponent::operator=(TransformComponent const &other) = default;

    TransformComponent &TransformComponent::operator=(TransformComponent &&other) noexcept = default;

    TransformComponent::~TransformComponent() = default;

    vec3f TransformComponent::getForward() const {
        vec3f const eulerRot{ quaternionToEuler(getRotation(TransformSpace::World)) };

        vec3f front;
        front.x = sin(eulerRot.y) * cos(eulerRot.x);
        front.y = sin(eulerRot.x);
        front.z = cos(eulerRot.y) * cos(eulerRot.x);

        return normalise(front);
    }

    vec3f TransformComponent::getRight() const {
        vec3f constexpr worldUp{ 0.0f, 1.0f, 0.0f };
        return normalise(cross(getForward(), worldUp));
    }

    vec3f TransformComponent::getUp() const {
        return normalise(cross(getRight(), getForward()));
    }

    mat4f TransformComponent::getWorldTransformMatrix() const {
        return getLocalTransformMatrix();
    }

    mat4f TransformComponent::getLocalTransformMatrix() const {
        mat4f const translationMatrix{ translate(mat4f{ 1.0f }, localPosition) };
        mat4f const rotationMatrix{ quaternionToMatrix4(localRotation) };
        mat4f const scaleMatrix{ scale(mat4f{ 1.0f }, localScale) };

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    vec3f TransformComponent::getWorldPosition() const {
        mat4f const transformMatrix{ getWorldTransformMatrix() };

        return { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
    }

    const vec3f &TransformComponent::getLocalPosition() const {
        return localPosition;
    }

    quatf TransformComponent::getWorldRotation() const {
        mat4f transformMatrix{ getWorldTransformMatrix() };

        transformMatrix[3][0] = 0.0f;
        transformMatrix[3][1] = 0.0f;
        transformMatrix[3][2] = 0.0f;

        vec3f const scaleX{ transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
        vec3f const scaleY{ transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
        vec3f const scaleZ{ transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

        vec3f const scale{ length(scaleX), length(scaleY), length(scaleZ) };

        transformMatrix[0][0] /= scale.x;
        transformMatrix[0][1] /= scale.x;
        transformMatrix[0][2] /= scale.x;

        transformMatrix[1][0] /= scale.y;
        transformMatrix[1][1] /= scale.y;
        transformMatrix[1][2] /= scale.y;

        transformMatrix[2][0] /= scale.z;
        transformMatrix[2][1] /= scale.z;
        transformMatrix[2][2] /= scale.z;

        return matrixToQuaternion(transformMatrix);
    }

    quatf const &TransformComponent::getLocalRotation() const {
        return localRotation;
    }

    vec3f TransformComponent::getWorldScale() const {
        mat4f const transformMatrix{ getWorldTransformMatrix() };

        vec3f const scaleX{ transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
        vec3f const scaleY{ transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
        vec3f const scaleZ{ transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

        return { length(scaleX), length(scaleY), length(scaleZ) };
    }

    vec3f const &TransformComponent::getLocalScale() const {
        return localScale;
    }

    void TransformComponent::setWorldPosition(vec3f position) {
        setLocalPosition(std::move(position));
    }

    void TransformComponent::setLocalPosition(vec3f position) {
        localPosition = std::move(position);
    }

    void TransformComponent::setWorldRotation(quatf rotation) {
        setLocalRotation(std::move(rotation));
    }

    void TransformComponent::setLocalRotation(quatf rotation) {
        localRotation = std::move(rotation);
    }

    void TransformComponent::setWorldScale(vec3f scale) {
        setLocalScale(std::move(scale));
    }

    void TransformComponent::setLocalScale(vec3f scale) {
        localScale = std::move(scale);
    }
}