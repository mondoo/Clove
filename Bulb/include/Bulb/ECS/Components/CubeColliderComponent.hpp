#pragma once

#include "Bulb/ECS/Component.hpp"

class btBoxShape;
class btGhostObject;

namespace blb::ecs {
    /**
     * @brief Provides a cube collision shape for an entity.
     * @details On it's own a CubeColliderComponent can act like a trigger.
     * If an Entity also has a RigidBodyComponent then the Entity will respond
     * to gravity and other physics events using this component as it's collision
     * shape.
     */
    class CubeColliderComponent : public Component<CubeColliderComponent> {
        friend class PhysicsSystem;

        //VARIABLES
    private:
        std::unique_ptr<btBoxShape> collisionShape;
        std::unique_ptr<btGhostObject> collisionObject; /**< Acts as a stand in until the Entity has a RigidBodyComponent. */

        clv::mth::vec3f cubeSize;

        //FUNCTIONS
    public:
        CubeColliderComponent() = delete;
        CubeColliderComponent(clv::mth::vec3f cubeSize);

        CubeColliderComponent(const CubeColliderComponent& other);
        CubeColliderComponent(CubeColliderComponent&& other) noexcept;

        CubeColliderComponent& operator=(const CubeColliderComponent& other);
        CubeColliderComponent& operator=(CubeColliderComponent&& other) noexcept;

        ~CubeColliderComponent();

    private:
        void initialiseCubeCollider();
    };
}