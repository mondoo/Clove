#pragma once

#include <Clove/ECS/Component.hpp>

#include <Clove/Maths/MathsTypes.hpp>
#include <vector>

namespace garlic::clove {
    enum class TransformSpace {
        Local,
        World
    };
}

namespace garlic::clove {
    /**
     * @brief A TransformComponent holds the position, rotation and scale of an Entity.
     */
    class TransformComponent : public Component<TransformComponent> {
        //VARIABLES
    private:
        vec3f localPosition{ 0.0f, 0.0f, 0.0f };
        quatf localRotation{ quatf({ 0.0f, 0.0f, 0.0f }) };
        vec3f localScale{ 1.0f, 1.0f, 1.0f };

        //FUNCTIONS
    public:
        TransformComponent();

        TransformComponent(TransformComponent const &other);
        TransformComponent(TransformComponent &&other) noexcept;

        TransformComponent &operator=(TransformComponent const &other);
        TransformComponent &operator=(TransformComponent &&other) noexcept;

        ~TransformComponent();

        inline vec3f getPosition(TransformSpace space = TransformSpace::World) const;
        inline quatf getRotation(TransformSpace space = TransformSpace::World) const;
        inline vec3f getScale(TransformSpace space = TransformSpace::World) const;

        inline void setPosition(vec3f position, TransformSpace space = TransformSpace::World);
        inline void setRotation(quatf rotation, TransformSpace space = TransformSpace::World);
        inline void setScale(vec3f scale, TransformSpace space = TransformSpace::World);

        vec3f getForward() const;
        vec3f getRight() const;
        vec3f getUp() const;

        inline mat4f getTransformationMatrix(TransformSpace space) const;

    private:
        mat4f getWorldTransformMatrix() const;
        mat4f getLocalTransformMatrix() const;

        vec3f getWorldPosition() const;
        vec3f const &getLocalPosition() const;

        quatf getWorldRotation() const;
        quatf const &getLocalRotation() const;

        vec3f getWorldScale() const;
        vec3f const &getLocalScale() const;

        void setWorldPosition(vec3f position);
        void setLocalPosition(vec3f position);

        void setWorldRotation(quatf rotation);
        void setLocalRotation(quatf rotation);

        void setWorldScale(vec3f scale);
        void setLocalScale(vec3f scale);
    };
}

#include "TransformComponent.inl"