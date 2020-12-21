#pragma once

namespace garlic::clove {
    /**
	 * @brief Enables an entity to respond to physics events.
	 * @details Entities with a RigidBodyComponent can collide with other
	 * entities and be affected by gravity. If the entity also has a
	 * CollisionShapeComponent then the rigid body will use that shape
	 * to detect collisions.
	 */
    struct RigidBodyComponent {
        float mass{ 1.0f }; /**< The mass of the body. If 0 then the body will be able to be collided with but not respond to physics. */

        uint32_t collisionGroup{ ~0u }; /**< Bit flag of the collision groups this body is a part of. */
        uint32_t collisionMask{ ~0u };  /**< Bit flag of which collision groups this body collides with. */
    };
}
