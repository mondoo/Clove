#pragma once

#include "Bulb/ECS/System.hpp"
#include "Bulb/ECS/ECSEvents.hpp"

#include <Clove/Event/EventHandle.hpp>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace blb::ecs {
	class RigidBodyComponent;
}

namespace blb::ecs {
	class PhysicsSystem : public System {
		//VARIABLES
	private:
        btDefaultCollisionConfiguration* collisionConfiguration{ nullptr };
        btCollisionDispatcher* dispatcher{ nullptr };
        btBroadphaseInterface* overlappingPairCache{ nullptr };
        btSequentialImpulseConstraintSolver* solver{ nullptr };

        btDiscreteDynamicsWorld* dynamicsWorld{ nullptr };

		clv::EventHandle componentAddedHandle;
		clv::EventHandle componentRemovedHandle;

		//FUNCTIONS
	public:
		PhysicsSystem();

		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;

		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;

		~PhysicsSystem();

		void registerToEvents(clv::EventDispatcher& dispatcher) override;

		void update(World& world, clv::utl::DeltaTime deltaTime) override;

		//TODO
		//RigidBody* rayCast(const clv::mth::vec3f& begin, const clv::mth::vec3f& end);

	private:
		void onComponentAdded(const ComponentAddedEvent<RigidBodyComponent>& event);
		void onComponentRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event);
	};
}