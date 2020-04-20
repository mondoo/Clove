#pragma once

#include "Bulb/ECS/Component.hpp"

namespace blb::ai {
	class BehaviourTree;
}

namespace blb::ecs {
	class BehaviourTreeComponent : public Component<BehaviourTreeComponent> {
		friend class BehaviourTreeSystem;

		//VARIABLES
	private:
		std::unique_ptr<ai::BehaviourTree> behaviourTree;

		//FUNCTIONS
	public:
		BehaviourTreeComponent();
		BehaviourTreeComponent(std::unique_ptr<ai::BehaviourTree> behaviourTree);

		BehaviourTreeComponent(const BehaviourTreeComponent& other) = delete; //TODO: Implement copy ctors
		BehaviourTreeComponent(BehaviourTreeComponent&& other) noexcept;

		BehaviourTreeComponent& operator=(const BehaviourTreeComponent& other) = delete;
		BehaviourTreeComponent& operator=(BehaviourTreeComponent&& other) noexcept;

		~BehaviourTreeComponent();

		void setBehaviourTree(std::unique_ptr<ai::BehaviourTree> behaviourTree);
	};
}