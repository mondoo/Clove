#pragma once

#include "Tunic/ECS/System.hpp"

namespace tnc::phy {
	class World;
}

namespace tnc::ecs {
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

		virtual ~PhysicsSystem();

		virtual void update(World& world, clv::utl::DeltaTime deltaTime) override;

		virtual void onComponentCreated(ComponentInterface* component) override;
		virtual void onComponentDestroyed(ComponentInterface* component) override;
	};
}