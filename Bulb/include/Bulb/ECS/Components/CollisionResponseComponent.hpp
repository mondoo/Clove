#pragma once

#include "Bulb/ECS/Component.hpp"
#include "Bulb/ECS/Entity.hpp"

#include <Clove/Delegate/MultiCastDelegate.hpp>

namespace blb::ecs {
    struct Collision {
        Entity collider; /**< The Entity that collided with this entity */
        Entity collidee; /**< 'this' Entity. */
    };

    /**
     * @brief A CollisionResponseComponent exposes collision callbacks for an Entity.
     * @details Everytime a callback is triggered means another entity has collided
     * with this entity. Note that on it's own this component does nothing. The Entity
     * needs a RigidBodyComponent or a ColliderComponent to response to collisions.
     */
    class CollisionResponseComponent : public Component<CollisionResponseComponent> {
        //VARIABLES
    public:
        clv::MultiCastDelegate<void(const Collision&)> onCollision;
    };
}