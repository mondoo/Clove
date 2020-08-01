#pragma once

class btCollisionShape;
class btRigidBody;

namespace blb::phy {
    struct RigidBodyDescriptor {
        float mass        = 1.0f;
        float isKinematic = false; /**< If true, stops the body being affected by gravity */

        uint32_t collisionGroup = 0; /**< Bit flag of the collision groups this body is a part of */
        uint32_t collisionMask  = 0; /**< Bit flag of which collision groups this body collides with */
    };
}

namespace blb::phy {
    /**
	 * @brief A RigidBody is an object that can be collided with but does not deform.
	 */
    class RigidBody {
        friend class World;

        //VARIABLES
    private:
        std::unique_ptr<btCollisionShape> collisionShape;
        std::unique_ptr<btRigidBody> body;

        RigidBodyDescriptor descriptor{};

        clv::mth::vec3f cubeSize{};

        void* userPointer{ nullptr };

        //FUNCTIONS
    public:
        RigidBody() = delete;
        //Only supporting box shapes
        RigidBody(RigidBodyDescriptor initInfo, const clv::mth::vec3f& cubeSize);

        RigidBody(const RigidBody& other);
        RigidBody(RigidBody&& other) noexcept;

        RigidBody& operator=(const RigidBody& other);
        RigidBody& operator=(RigidBody&& other) noexcept;

        ~RigidBody();

        void setWorldPosition(const clv::mth::vec3f& position);
        void setWorldRotation(const clv::mth::quatf& rotation);

        void setLinearVelocity(const clv::mth::vec3f& velocity);

        void applyForce(const clv::mth::vec3f& force, const clv::mth::vec3f& relativeOffset = { 0.0f, 0.0f, 0.0f });
        void applyImpulse(const clv::mth::vec3f& impulse, const clv::mth::vec3f& relativeOffset = { 0.0f, 0.0f, 0.0f });

        clv::mth::vec3f getPhysicsPosition() const;
        clv::mth::quatf getPhysicsRotation() const;

        clv::mth::vec3f getLinearVelocity() const;

        void setUserPointer(void* pointer);
        void* getUserPointer() const;

    private:
        void initialise(const RigidBodyDescriptor& descriptor, const clv::mth::vec3f& cubeSize);
    };
}