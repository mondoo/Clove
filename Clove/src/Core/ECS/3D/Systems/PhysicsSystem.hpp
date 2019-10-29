#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/3D/Components/RigidBodyComponent.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace clv::ecs::_3D{
	class PhysicsSystem : public System<RigidBodyComponent, TransformComponent>{
		//VARIABLES
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;

		btDiscreteDynamicsWorld* dynamicsWorld;

		//FUNCTIONS
	public:
		PhysicsSystem();
		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;
		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;
		virtual ~PhysicsSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		virtual void handleEntityCreation(const ComponentTuple& componentTuple) override;
		virtual void handleEntityDestruction(const ComponentTuple& componentTuple) override;
	};
}