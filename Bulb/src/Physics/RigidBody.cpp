#include "Bulb/Physics/RigidBody.hpp"

#include <btBulletDynamicsCommon.h>

namespace blb::phy {
	RigidBody::RigidBody(RigidBodyDescriptor descriptor, const clv::mth::vec3f& cubeSize)
        : descriptor(std::move(descriptor))
		, cubeSize(cubeSize) {
        initialise(this->descriptor, cubeSize);
	}

	RigidBody::RigidBody(const RigidBody& other) {
        descriptor  = std::move(other.descriptor);
        cubeSize    = other.cubeSize;
        userPointer = other.userPointer;

        initialise(descriptor, cubeSize);
	}

	RigidBody::RigidBody(RigidBody&& other) noexcept = default;

	RigidBody& RigidBody::operator=(const RigidBody& other) {
        descriptor  = std::move(other.descriptor);
        cubeSize    = other.cubeSize;
        userPointer = other.userPointer;

        initialise(descriptor, cubeSize);

		return *this;
	}

	RigidBody& RigidBody::operator=(RigidBody&& other) noexcept = default;

	RigidBody::~RigidBody() = default;

	void RigidBody::setWorldPosition(const clv::mth::vec3f& position) {
		btTransform btTrans = body->getWorldTransform();
		btTrans.setOrigin({ position.x, position.y, position.z });
		body->setWorldTransform(btTrans);
	}

	void RigidBody::setWorldRotation(const clv::mth::quatf& rotation) {
		btTransform btTrans = body->getWorldTransform();
		btTrans.setRotation({ rotation.x, rotation.y, rotation.z, rotation.w }); //GLM is pitch yaw roll while Bullet is yaw pitch roll
		body->setWorldTransform(btTrans);
	}

	void RigidBody::setLinearVelocity(const clv::mth::vec3f& velocity) {
		const btVector3 btvel{ velocity.x, velocity.y, velocity.z };
		body->setLinearVelocity(btvel);
		body->activate();
    }

    clv::mth::vec3f RigidBody::getLinearVelocity() const {
        const btVector3 btvel = body->getLinearVelocity();
        return { btvel.x(), btvel.y(), btvel.z() };
    }

	clv::mth::vec3f RigidBody::getPhysicsPosition() const {
		btTransform btTrans = body->getWorldTransform();
		const auto pos = btTrans.getOrigin();
		return { pos.getX(), pos.getY(), pos.getZ() };
	}

	clv::mth::quatf RigidBody::getPhysicsRotation() const {
		btTransform btTrans = body->getWorldTransform();
		const auto rot = btTrans.getRotation();
		return { rot.getW(), rot.getX(), rot.getY(), rot.getZ() }; //GLM is pitch yaw roll while Bullet is yaw pitch roll
	}

	void RigidBody::setUserPointer(void* pointer) {
		userPointer = pointer;
	}

	void* RigidBody::getUserPointer() const {
		return userPointer;
	}

	void RigidBody::initialise(const RigidBodyDescriptor& descriptor, const clv::mth::vec3f& cubeSize) {
		collisionShape = std::make_unique<btBoxShape>(btVector3{ cubeSize.x, cubeSize.y, cubeSize.z });

		btVector3 localInertia(0, 0, 0);
		btTransform startTransform;
		startTransform.setIdentity();

		if(descriptor.isKinematic && descriptor.mass > 0.0f) {
			GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Kinematic RigidBody has non 0 mass. Kinematic takes precedence");
		} else {
            collisionShape->calculateLocalInertia(descriptor.mass, localInertia);
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(descriptor.mass, myMotionState, collisionShape.get(), localInertia);

		body = std::make_unique<btRigidBody>(rbInfo);
		body->setUserPointer(this);

		int flags = body->getCollisionFlags();
        if(descriptor.isKinematic) {
			flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		}
		body->setCollisionFlags(flags);
	}
}