#include "Tunic/ECS/Components/BehaviourTreeComponent.hpp"

namespace tnc::ecs {
	BehaviourTreeComponent::BehaviourTreeComponent() = default;

	BehaviourTreeComponent::BehaviourTreeComponent(std::unique_ptr<ai::BehaviourTree> behaviourTree) 
		: behaviourTree(std::move(behaviourTree)) {
	}

	BehaviourTreeComponent::BehaviourTreeComponent(BehaviourTreeComponent&& other) noexcept = default;

	BehaviourTreeComponent& BehaviourTreeComponent::operator=(BehaviourTreeComponent&& other) noexcept = default;

	BehaviourTreeComponent::~BehaviourTreeComponent() = default;

	void BehaviourTreeComponent::setBehaviourTree(std::unique_ptr<ai::BehaviourTree> behaviourTree) {
		this->behaviourTree = std::move(behaviourTree);
	}
}