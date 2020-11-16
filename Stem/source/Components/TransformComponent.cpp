#include "Stem/Components/TransformComponent.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

using namespace clv;
using namespace blb::ecs;

namespace garlic::inline stem {
    static void removeItemFromVector(ComponentPtr<TransformComponent> const &item, std::vector<ComponentPtr<TransformComponent>> &vector) {
        auto removeIter = std::remove(vector.begin(), vector.end(), item);
        vector.erase(removeIter);
    }

    TransformComponent::TransformComponent() = default;

    TransformComponent::TransformComponent(TransformComponent const &other) {
        localPosition = other.localPosition;
        localRotation = other.localRotation;
        localScale    = other.localScale;

        parent = other.parent;
        //Not copying children for now, transform component has no knowledge of other components
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "{0}: Unable to copy transform's children.", GARLIC_FUNCTION_NAME_PRETTY);

        if(parent != nullptr) {
            parent->children.push_back(this);
        }
    }

    TransformComponent::TransformComponent(TransformComponent &&other) noexcept = default;

    TransformComponent &TransformComponent::operator=(TransformComponent const &other) {
        localPosition = other.localPosition;
        localRotation = other.localRotation;
        localScale    = other.localScale;

        parent = other.parent;
        //Not copying children for now, transform component has no knowledge of other components
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "{0}: Unable to copy transform's children.", GARLIC_FUNCTION_NAME_PRETTY);

        if(parent != nullptr) {
            parent->children.push_back(this);
        }

        return *this;
    }

    TransformComponent &TransformComponent::operator=(TransformComponent &&other) noexcept = default;

    TransformComponent::~TransformComponent() {
        if(parent != nullptr) {
            removeItemFromVector(this, parent->children);
        }

        for(auto &&child : children) {
            child->parent.reset();
        }
    }

    mth::vec3f TransformComponent::getForward() const {
        mth::vec3f eulerRot = mth::quaternionToEuler(getRotation(TransformSpace::World));

        mth::vec3f front;
        front.x = sin(eulerRot.y) * cos(eulerRot.x);
        front.y = sin(eulerRot.x);
        front.z = cos(eulerRot.y) * cos(eulerRot.x);

        return mth::normalise(front);
    }

    mth::vec3f TransformComponent::getRight() const {
        mth::vec3f constexpr worldUp{ 0.0f, 1.0f, 0.0f };
        return mth::normalise(mth::cross(getForward(), worldUp));
    }

    mth::vec3f TransformComponent::getUp() const {
        return mth::normalise(mth::cross(getRight(), getForward()));
    }

    void TransformComponent::addChild(ComponentPtr<TransformComponent> child) {
        if(child != nullptr && child != this) {
            children.push_back(child);
            if(child->parent != nullptr) {
                removeItemFromVector(child, child->parent->children);
            }
            child->parent = this;
        }
    }

    void TransformComponent::removeChild(ComponentPtr<TransformComponent> child) {
        if(child != nullptr && child != this && child->parent == this) {
            removeItemFromVector(child, children);
            child->parent = nullptr;
        }
    }

    std::vector<ComponentPtr<TransformComponent>> const &TransformComponent::getChildren() const {
        return children;
    }

    mth::mat4f TransformComponent::getWorldTransformMatrix() const {
        if(parent != nullptr) {
            return parent->getWorldTransformMatrix() * getLocalTransformMatrix();
        } else {
            return getLocalTransformMatrix();
        }
    }

    mth::mat4f TransformComponent::getLocalTransformMatrix() const {
        mth::mat4f const translationMatrix{ mth::translate(mth::mat4f{ 1.0f }, localPosition) };
        mth::mat4f const rotationMatrix{ mth::quaternionToMatrix4(localRotation) };
        mth::mat4f const scaleMatrix{ mth::scale(mth::mat4f{ 1.0f }, localScale) };

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    mth::vec3f TransformComponent::getWorldPosition() const {
        mth::mat4f const transformMatrix{ getWorldTransformMatrix() };

        return { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
    }

    const mth::vec3f &TransformComponent::getLocalPosition() const {
        return localPosition;
    }

    mth::quatf TransformComponent::getWorldRotation() const {
        mth::mat4f transformMatrix{ getWorldTransformMatrix() };

        transformMatrix[3][0] = 0.0f;
        transformMatrix[3][1] = 0.0f;
        transformMatrix[3][2] = 0.0f;

        mth::vec3f const scaleX{ transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
        mth::vec3f const scaleY{ transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
        mth::vec3f const scaleZ{ transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

        mth::vec3f const scale{ mth::length(scaleX), mth::length(scaleY), mth::length(scaleZ) };

        transformMatrix[0][0] /= scale.x;
        transformMatrix[0][1] /= scale.x;
        transformMatrix[0][2] /= scale.x;

        transformMatrix[1][0] /= scale.y;
        transformMatrix[1][1] /= scale.y;
        transformMatrix[1][2] /= scale.y;

        transformMatrix[2][0] /= scale.z;
        transformMatrix[2][1] /= scale.z;
        transformMatrix[2][2] /= scale.z;

        return mth::matrixToQuaternion(transformMatrix);
    }

    mth::quatf const &TransformComponent::getLocalRotation() const {
        return localRotation;
    }

    mth::vec3f TransformComponent::getWorldScale() const {
        mth::mat4f const transformMatrix{ getWorldTransformMatrix() };

        mth::vec3f const scaleX{ transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2] };
        mth::vec3f const scaleY{ transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2] };
        mth::vec3f const scaleZ{ transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2] };

        return { mth::length(scaleX), mth::length(scaleY), mth::length(scaleZ) };
    }

    mth::vec3f const &TransformComponent::getLocalScale() const {
        return localScale;
    }

    void TransformComponent::setWorldPosition(clv::mth::vec3f position) {
        if(parent != nullptr) {
            setLocalPosition(position - parent->getPosition());
        } else {
            setLocalPosition(std::move(position));
        }
    }

    void TransformComponent::setLocalPosition(mth::vec3f position) {
        localPosition = std::move(position);
    }

    void TransformComponent::setWorldRotation(mth::quatf rotation) {
        if(parent != nullptr) {
            mth::mat4f const rotMat{ mth::quaternionToMatrix4(std::move(rotation)) };
            mth::mat4f const parentRotMat{ mth::quaternionToMatrix4(parent->getRotation()) };
            mth::mat4f const adjustedRot{ parentRotMat / rotMat };

            setLocalRotation(mth::matrixToQuaternion(adjustedRot));
        } else {
            setLocalRotation(std::move(rotation));
        }
    }

    void TransformComponent::setLocalRotation(mth::quatf rotation) {
        localRotation = std::move(rotation);
    }

    void TransformComponent::setWorldScale(mth::vec3f scale) {
        if(parent != nullptr) {
            setLocalScale(scale / parent->getScale());
        } else {
            setLocalScale(std::move(scale));
        }
    }

    void TransformComponent::setLocalScale(mth::vec3f scale) {
        localScale = std::move(scale);
    }
}