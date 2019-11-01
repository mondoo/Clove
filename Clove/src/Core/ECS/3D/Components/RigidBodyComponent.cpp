#include "RigidBodyComponent.hpp"

#include <btBulletDynamicsCommon.h>

namespace clv::ecs::_3D{
	RigidBodyComponent::RigidBodyComponent(){
		initialise();
	}

	RigidBodyComponent::RigidBodyComponent(RigidBodyComponent&& other) noexcept{
		onBodyCollision = std::move(other.onBodyCollision);
		
		collisionShape = std::move(other.collisionShape);
		body = std::move(other.body);

		mass = std::move(other.mass);
		isKinematic = std::move(other.isKinematic);
	}

	RigidBodyComponent& RigidBodyComponent::operator=(RigidBodyComponent&& other) noexcept{
		onBodyCollision = std::move(other.onBodyCollision);

		collisionShape = std::move(other.collisionShape);
		body = std::move(other.body);

		mass = std::move(other.mass);
		isKinematic = std::move(other.isKinematic);

		return *this;
	}

	RigidBodyComponent::~RigidBodyComponent() = default;

	RigidBodyComponent::RigidBodyComponent(float mass, bool isKinematic)
		: mass(mass)
		, isKinematic(isKinematic){
		initialise();
	}

	void RigidBodyComponent::initialise(){
		//TODO: Make shape around correct size
		//Probably manually specify like most things do
		collisionShape = std::make_unique<btBoxShape>(btVector3(1, 1, 1));

		btVector3 localInertia(0, 0, 0);
		btTransform startTransform;
		startTransform.setIdentity();

		//Test
		if(!isKinematic){
			collisionShape->calculateLocalInertia(mass, localInertia);
			startTransform.setOrigin(btVector3(0, 10, 0));
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape.get(), localInertia);

		body = std::make_unique<btRigidBody>(rbInfo);
		body->setUserPointer(this);
		if(isKinematic){
			body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		}
	}
}