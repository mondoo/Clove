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
		//TODO: Make shape around correct size
		//Probably manually specify like most things do
		collisionShape = new btBoxShape(btVector3(1, 1, 1));
		
		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape);
		rigidBody = new btRigidBody(rbInfo);
	}
}