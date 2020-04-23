#pragma once

#include "Bulb/ECS/System.hpp"

namespace blb::phy {
	class World;
}

namespace blb::ecs {
	class PhysicsSystem : public System {
		//VARIABLES
	private:
		std::unique_ptr<phy::World> physicsWorld;

		//FUNCTIONS
	public:
		PhysicsSystem();
		PhysicsSystem(std::unique_ptr<phy::World> physicsWorld);

		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;

		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;

		~PhysicsSystem();

		void update(World& world, clv::utl::DeltaTime deltaTime) override;

		void onComponentCreated(ComponentInterface* component) override;
		void onComponentDestroyed(ComponentInterface* component) override;
	};
}