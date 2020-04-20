#include "Bulb/Physics/RigidBody.hpp"

#include <btBulletDynamicsCommon.h>

namespace blb::phy{
	RigidBody::RigidBody(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize)
		: mass(mass), isKinematic(isKinematic), respondToCollision(respondToCollision), cubeSize(cubeSize){
		initialise(mass, isKinematic, respondToCollision, cubeSize);
	}

	RigidBody::RigidBody(const RigidBody& other){
		mass = other.mass;
		isKinematic = other.isKinematic;
		respondToCollision = other.respondToCollision;

		cubeSize = other.cubeSize;

		userPointer = other.userPointer;

		initialise(mass, isKinematic, respondToCollision, cubeSize);
	}

	RigidBody::RigidBody(RigidBody&& other) noexcept = default;

	RigidBody& RigidBody::operator=(const RigidBody& other){
		mass = other.mass;
		isKinematic = other.isKinematic;
		respondToCollision = other.respondToCollision;

		cubeSize = other.cubeSize;

		userPointer = other.userPointer;

		initialise(mass, isKinematic, respondToCollision, cubeSize);

		return *this;
	}

	RigidBody& RigidBody::operator=(RigidBody&& other) noexcept = default;

	RigidBody::~RigidBody() = default;

	void RigidBody::setWorldPosition(const clv::mth::vec3f& position){
		btTransform btTrans = body->getWorldTransform();
		btTrans.setOrigin({ position.x, position.y, position.z });
		body->setWorldTransform(btTrans);
	}

	void RigidBody::setWorldRotation(const clv::mth::quatf& rotation){
		btTransform btTrans = body->getWorldTransform();
		btTrans.setRotation({ rotation.x, rotation.y, rotation.z, rotation.w }); //GLM is pitch yaw roll while Bullet is yaw pitch roll
		body->setWorldTransform(btTrans);
	}

	clv::mth::vec3f RigidBody::getPhysicsPosition() const{
		btTransform btTrans = body->getWorldTransform();
		const auto pos = btTrans.getOrigin();
		return { pos.getX(), pos.getY(), pos.getZ() };
	}

	clv::mth::quatf RigidBody::getPhysicsRotation() const{
		btTransform btTrans = body->getWorldTransform();
		const auto rot = btTrans.getRotation();
		return { rot.getW(), rot.getX(), rot.getY(), rot.getZ() }; //GLM is pitch yaw roll while Bullet is yaw pitch roll
	}

	void RigidBody::setUserPointer(void* pointer){
		userPointer = pointer;
	}

	void* RigidBody::getUserPointer() const{
		return userPointer;
	}

	void RigidBody::initialise(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize){
		collisionShape = std::make_unique<btBoxShape>(btVector3{ cubeSize.x, cubeSize.y, cubeSize.z });

		btVector3 localInertia(0, 0, 0);
		btTransform startTransform;
		startTransform.setIdentity();

		if (isKinematic && mass > 0.0f){
			CLV_LOG_WARN("Kinematic body has non 0 mass. Setting to 0");
			mass = 0.0f;
		} else{
			collisionShape->calculateLocalInertia(mass, localInertia);
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape.get(), localInertia);

		body = std::make_unique<btRigidBody>(rbInfo);
		body->setUserPointer(this);

		int flags = body->getCollisionFlags();
		if (isKinematic){
			flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		}
		if (!respondToCollision){
			flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
		}
		body->setCollisionFlags(flags);
	}
}