#pragma once

#include "Bulb/ECS/System.hpp"

namespace blb::ecs {
	class BehaviourTreeSystem : public System {
		//FUNCTIONS
	public:
		BehaviourTreeSystem();

		BehaviourTreeSystem(const BehaviourTreeSystem& other) = delete;
		BehaviourTreeSystem(BehaviourTreeSystem&& other) noexcept;

		BehaviourTreeSystem& operator=(const BehaviourTreeSystem& other) = delete;
		BehaviourTreeSystem& operator=(BehaviourTreeSystem&& other) noexcept;

		~BehaviourTreeSystem();

		void update(World& world, clv::utl::DeltaTime deltaTime) override;
	};
}