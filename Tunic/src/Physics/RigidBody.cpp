#include "Tunic/Physics/RigidBody.hpp"

#include <btBulletDynamicsCommon.h>

namespace tnc::phy{
	RigidBody::RigidBody(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize)
		: mass(mass), isKinematic(isKinematic), respondToCollision(respondToCollision), cubeSize(cubeSize){

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

	RigidBody::RigidBody(RigidBody&& other) noexcept = default;

	RigidBody& RigidBody::operator=(RigidBody&& other) noexcept = default;

	RigidBody::~RigidBody() = default;
}