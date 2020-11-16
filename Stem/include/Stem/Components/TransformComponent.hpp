#pragma once

#include <Bulb/ECS/Component.hpp>

namespace garlic::inline stem {
    enum class TransformSpace {
        Local,
        World
    };
}

namespace garlic::inline stem {
    /**
     * @brief A TransformComponent holds the position, rotation and scale of an Entity.
     */
    class TransformComponent : public blb::ecs::Component<TransformComponent> {
        //VARIABLES
    private:
        clv::mth::vec3f localPosition{ 0.0f, 0.0f, 0.0f };
        clv::mth::quatf localRotation{ clv::mth::quatf({ 0.0f, 0.0f, 0.0f }) };
        clv::mth::vec3f localScale{ 1.0f, 1.0f, 1.0f };

        blb::ecs::ComponentPtr<TransformComponent> parent;
        std::vector<blb::ecs::ComponentPtr<TransformComponent>> children;

        //FUNCTIONS
    public:
        TransformComponent();

        TransformComponent(TransformComponent const &other);
        TransformComponent(TransformComponent &&other) noexcept;

        TransformComponent &operator=(TransformComponent const &other);
        TransformComponent &operator=(TransformComponent &&other) noexcept;

        ~TransformComponent();

        inline clv::mth::vec3f getPosition(TransformSpace space = TransformSpace::World) const;
        inline clv::mth::quatf getRotation(TransformSpace space = TransformSpace::World) const;
        inline clv::mth::vec3f getScale(TransformSpace space = TransformSpace::World) const;

        inline void setPosition(clv::mth::vec3f position, TransformSpace space = TransformSpace::World);
        inline void setRotation(clv::mth::quatf rotation, TransformSpace space = TransformSpace::World);
        inline void setScale(clv::mth::vec3f scale, TransformSpace space = TransformSpace::World);

        clv::mth::vec3f getForward() const;
        clv::mth::vec3f getRight() const;
        clv::mth::vec3f getUp() const;

        inline clv::mth::mat4f getTransformationMatrix(TransformSpace space);

        inline blb::ecs::ComponentPtr<TransformComponent> getParent() const;

        void addChild(blb::ecs::ComponentPtr<TransformComponent> child);
        void removeChild(blb::ecs::ComponentPtr<TransformComponent> child);
        std::vector<blb::ecs::ComponentPtr<TransformComponent>> const &getChildren() const;

    private:
        clv::mth::mat4f getWorldTransformMatrix() const;
        clv::mth::mat4f getLocalTransformMatrix() const;

        clv::mth::vec3f getWorldPosition() const;
        clv::mth::vec3f const &getLocalPosition() const;

        clv::mth::quatf getWorldRotation() const;
        clv::mth::quatf const &getLocalRotation() const;

        clv::mth::vec3f getWorldScale() const;
        clv::mth::vec3f const &getLocalScale() const;

        void setWorldPosition(clv::mth::vec3f position);
        void setLocalPosition(clv::mth::vec3f position);

        void setWorldRotation(clv::mth::quatf rotation);
        void setLocalRotation(clv::mth::quatf rotation);

        void setWorldScale(clv::mth::vec3f scale);
        void setLocalScale(clv::mth::vec3f scale);
    };
}

#include "TransformComponent.inl"