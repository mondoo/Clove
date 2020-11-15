#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace garlic::inline stem {
    clv::mth::vec3f TransformComponent::getPosition(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalPosition();
            case TransformSpace::World:
                return getWorldPosition();
            default:
                GARLIC_ASSERT(false, "Default statement hit in {0}", GARLIC_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    clv::mth::quatf TransformComponent::getRotation(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalRotation();
            case TransformSpace::World:
                return getWorldRotation();
            default:
                GARLIC_ASSERT(false, "Default statement hit in {0}", GARLIC_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    clv::mth::vec3f TransformComponent::getScale(TransformSpace space) const {
        switch(space) {
            case TransformSpace::Local:
                return getLocalScale();
            case TransformSpace::World:
                return getWorldScale();
            default:
                GARLIC_ASSERT(false, "Default statement hit in {0}", GARLIC_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    void TransformComponent::setPosition(clv::mth::vec3f position, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalPosition(std::move(position));
                break;
            case TransformSpace::World:
                setWorldPosition(std::move(position));
                break;
        }
    }

    void TransformComponent::setRotation(clv::mth::quatf rotation, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalRotation(std::move(rotation));
                break;
            case TransformSpace::World:
                setWorldRotation(std::move(rotation));
                break;
        }
    }

    void TransformComponent::setScale(clv::mth::vec3f scale, TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                setLocalScale(std::move(scale));
                break;
            case TransformSpace::World:
                setWorldScale(std::move(scale));
                break;
        }
    }

    clv::mth::mat4f TransformComponent::getTransformationMatrix(TransformSpace space) {
        switch(space) {
            case TransformSpace::Local:
                return getLocalTransformMatrix();
            case TransformSpace::World:
                return getWorldTransformMatrix();
            default:
                GARLIC_ASSERT(false, "Default statement hit in {0}", GARLIC_FUNCTION_NAME_PRETTY);
                return {};
        }
    }

    blb::ecs::ComponentPtr<TransformComponent> TransformComponent::getParent() const {
        return parent;
    }
}