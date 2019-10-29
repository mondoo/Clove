#include "RigidBodyComponent.hpp"

#include <btBulletDynamicsCommon.h>

namespace clv::ecs::_3D{
	RigidBodyComponent::RigidBodyComponent(){
		initialise();
	}

	RigidBodyComponent::RigidBodyComponent(RigidBodyComponent &&other) noexcept = default;

	RigidBodyComponent &RigidBodyComponent::operator=(RigidBodyComponent &&other) noexcept = default;

	RigidBodyComponent::~RigidBodyComponent(){
		delete collisionShape;
		delete rigidBody;
	}

	RigidBodyComponent::RigidBodyComponent(float mass)
		: mass(mass){
		initialise();
	}

	void RigidBodyComponent::applyForce(math::Vector3f force){
		this->force = std::move(force);
	}

	void RigidBodyComponent::initialise(){
		collisionShape = new btBoxShape(btVector3(1, 1, 1));
		//TODO: Make shape around correct size
		//Probably manually specify like most things do

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(this->mass);

		btVector3 localInertia(0, 0, 0);
		if(mass != 0.0f){
			collisionShape->calculateLocalInertia(mass, localInertia);
		}

		startTransform.setOrigin(btVector3(0, 0, 0));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
		rigidBody = new btRigidBody(rbInfo);
	}
}