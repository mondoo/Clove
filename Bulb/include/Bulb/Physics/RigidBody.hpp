#pragma once

#include <any>

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

        std::any userData{};

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

        void setRestitution(float restitution);

        /**
         * @brief Set the factor which rotations are multiplied against.
         * @details For example, giving a factor of {0.0f, 0.0f, 0.0f} would stop rotations entirely.
         */
        void setAngularFactor(const clv::mth::vec3f& factor);

        clv::mth::vec3f getPhysicsPosition() const;
        clv::mth::quatf getPhysicsRotation() const;

        clv::mth::vec3f getLinearVelocity() const;

        void setUserData(std::any data);
        std::any getUserData() const;

        float getRestitution() const;

        clv::mth::vec3f getAngularFactor() const;

    private:
        void initialise(const RigidBodyDescriptor& descriptor, const clv::mth::vec3f& cubeSize);
    };
}