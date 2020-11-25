#pragma once

#include <Clove/ECS/Component.hpp>
#include <memory>
#include <Clove/Maths/Vector.hpp>

class btSphereShape;
class btRigidBody;
class btDefaultMotionState;

namespace garlic::clove {
    /**
	 * @brief Enables an entity to respond to physics events.
	 * @details Entities with a RigidBodyComponent can collide with other
	 * entities and be affected by gravity. If the entity also has a
	 * CubeColliderComponent or similar the rigid body will use that shape
	 * to detect collisions.
	 */
    class RigidBodyComponent : public Component<RigidBodyComponent> {
        friend class PhysicsSystem;

        //TYPES
    public:
        struct Descriptor {
            float mass{ 1.0f };
            bool isKinematic{ false }; /**< If true, stops the body being affected by gravity */

            uint32_t collisionGroup{ 0 }; /**< Bit flag of the collision groups this body is a part of */
            uint32_t collisionMask{ 0 };  /**< Bit flag of which collision groups this body collides with */
        };

        //VARIABLES
    private:
        Descriptor descriptor;

        std::unique_ptr<btSphereShape> standInShape; /**< Stand in shape until a _ColliderComponent has been added */
        std::unique_ptr<btRigidBody> body;

        std::unique_ptr<btDefaultMotionState> motionState;

        //FUNCTIONS
    public:
        RigidBodyComponent();
        RigidBodyComponent(Descriptor descriptor);

        RigidBodyComponent(RigidBodyComponent const &other);
        RigidBodyComponent(RigidBodyComponent &&other) noexcept;

        RigidBodyComponent &operator=(RigidBodyComponent const &other);
        RigidBodyComponent &operator=(RigidBodyComponent &&other) noexcept;

        ~RigidBodyComponent();

        void setLinearVelocity(vec3f const &velocity);

        void applyForce(vec3f const &force, vec3f const &relativeOffset = { 0.0f, 0.0f, 0.0f });
        void applyImpulse(vec3f const &impulse, vec3f const &relativeOffset = { 0.0f, 0.0f, 0.0f });

        void setRestitution(float restitution);

        void setAngularFactor(vec3f const &factor);

        vec3f getLinearVelocity() const;

        float getRestitution() const;

        vec3f getAngularFactor() const;

    private:
        void initialiseRigidBody();

        static std::unique_ptr<btSphereShape> createStandInShape();
    };
}
