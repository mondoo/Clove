#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    vec3f TransformComponent::getPosition(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalPosition();
            case TransformSpace::World:
                return getWorldPosition();
            default:
                CLOVE_ASSERT(false, "Default statement hit in {0}", CLOVE_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    quatf TransformComponent::getRotation(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalRotation();
            case TransformSpace::World:
                return getWorldRotation();
            default:
                CLOVE_ASSERT(false, "Default statement hit in {0}", CLOVE_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    vec3f TransformComponent::getScale(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalScale();
            case TransformSpace::World:
                return getWorldScale();
            default:
                CLOVE_ASSERT(false, "Default statement hit in {0}", CLOVE_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    void TransformComponent::setPosition(vec3f position, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalPosition(std::move(position));
                break;
            case TransformSpace::World:
                setWorldPosition(std::move(position));
                break;
        }
    }

    void TransformComponent::setRotation(quatf rotation, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalRotation(std::move(rotation));
                break;
            case TransformSpace::World:
                setWorldRotation(std::move(rotation));
                break;
        }
    }

    void TransformComponent::setScale(vec3f scale, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalScale(std::move(scale));
                break;
            case TransformSpace::World:
                setWorldScale(std::move(scale));
                break;
        }
    }

    mat4f TransformComponent::getTransformationMatrix(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalTransformMatrix();
            case TransformSpace::World:
                return getWorldTransformMatrix();
            default:
                CLOVE_ASSERT(false, "Default statement hit in {0}", CLOVE_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    ComponentPtr<TransformComponent> TransformComponent::getParent() const {
        return parent;
    }
}