#pragma once

#include "Tunic/ECS/System.hpp"

namespace tnc::ecs {
	class BehaviourTreeSystem : public System {
		//FUNCTIONS
	public:
		BehaviourTreeSystem();

		BehaviourTreeSystem(const BehaviourTreeSystem& other) = delete;
		BehaviourTreeSystem(BehaviourTreeSystem&& other) noexcept;

		BehaviourTreeSystem& operator=(const BehaviourTreeSystem& other) = delete;
		BehaviourTreeSystem& operator=(BehaviourTreeSystem&& other) noexcept;

		~BehaviourTreeSystem();

		virtual void update(World& world, clv::utl::DeltaTime deltaTime) override;
	};
}