#pragma once

#include "Bulb/ECS/Component.hpp"

class btBoxShape;
class btCollisionObject;

namespace blb::ecs {
    /**
     * @brief Provides a cube collision shape for an entity.
     * @details On it's own a CubeColliderComponent can act like a trigger.
     * If an Entity also has a RigidBodyComponent then the Entity can respond
     * to gravity and other physics events using this component as it's collision
     * shape.
     */
    class CubeColliderComponent : public Component<CubeColliderComponent> {
        friend class PhysicsSystem;

        //VARIABLES
    private:
        std::unique_ptr<btBoxShape> collisionShape;
        std::shared_ptr<btCollisionObject> collisionObject; /**< Is either a btGhostObject or if there's a RigidBodyComponent a btRigidBody. */

        //FUNCTIONS
    public:
        CubeColliderComponent() = delete;
        CubeColliderComponent(const clv::mth::vec3f& cubeSize);
    };
}