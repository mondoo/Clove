#pragma once

#include <Bulb/ECS/Component.hpp>

class btBoxShape;
class btGhostObject;

namespace garlic::inline stem {
    /**
     * @brief Provides a cube collision shape for an entity.
     * @details On it's own a CubeColliderComponent can act like a trigger.
     * If an Entity also has a RigidBodyComponent then the Entity will respond
     * to gravity and other physics events using this component as it's collision
     * shape.
     */
    class CubeColliderComponent : public blb::ecs::Component<CubeColliderComponent> {
        friend class PhysicsSystem;

        //VARIABLES
    private:
        std::unique_ptr<btBoxShape> collisionShape;
        std::unique_ptr<btGhostObject> collisionObject; /**< Acts as a stand in until the Entity has a RigidBodyComponent. */

        clv::mth::vec3f cubeSize;

        //FUNCTIONS
    public:
        CubeColliderComponent();
        CubeColliderComponent(clv::mth::vec3f cubeSize);

        CubeColliderComponent(CubeColliderComponent const &other);
        CubeColliderComponent(CubeColliderComponent &&other) noexcept;

        CubeColliderComponent &operator=(CubeColliderComponent const &other);
        CubeColliderComponent &operator=(CubeColliderComponent &&other) noexcept;

        ~CubeColliderComponent();

        void setCubeSize(clv::mth::vec3f cubeSize);
        clv::mth::vec3f const &getCubeSize() const;

    private:
        void initialiseCubeCollider();

        void constructCollisionObject();
    };
}