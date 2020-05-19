#pragma once

#include "Bulb/ECS/System.hpp"
#include "Bulb/ECS/ECSEvents.hpp"

#include <Clove/Event/EventHandle.hpp>

namespace blb::phy {
	class World;
}

namespace blb::ecs {
	class RigidBodyComponent;
}

namespace blb::ecs {
	class PhysicsSystem : public System {
		//VARIABLES
	private:
		std::unique_ptr<phy::World> physicsWorld;

		clv::EventHandle componentAddedHandle;
		clv::EventHandle componentRemovedHandle;

		//FUNCTIONS
	public:
		PhysicsSystem();
		PhysicsSystem(std::unique_ptr<phy::World> physicsWorld);

		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;

		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;

		~PhysicsSystem();

		void registerToEvents(clv::EventDispatcher& dispatcher) override;

		void update(World& world, clv::utl::DeltaTime deltaTime) override;

	private:
		void onComponentAdded(const ComponentAddedEvent<RigidBodyComponent>& event);
		void onComponentRemoved(const ComponentRemovedEvent<RigidBodyComponent>& event);
	};
}