#pragma once

#include "Bulb/ECS/Component.hpp"

class btCollisionShape;
class btCollisionObject;

namespace blb::ecs {
    /**
     * @brief Provides a cube collision shape for an entity.
     * @details On it's own a CubeColliderComponent can act like a trigger.
     * If an Entity also has a RigidBodyComponent then the Entity can respond
     * to gravity and other physics events.
     */
    class CubeColliderComponent : public Component<CubeColliderComponent> {
        friend class PhysicsSystem;

        //VARIABLES
    private:
        std::unique_ptr<btCollisionShape> collisionShape;
        std::shared_ptr<btCollisionObject> collisionObject;

        //FUNCTIONS
    public:
        CubeColliderComponent() = delete;
        CubeColliderComponent(const clv::mth::vec3f& cubeSize);
    };
}