#include "Tunic/ECS/Systems/BehaviourTreeSystem.hpp"

#include "Tunic/AI/BehaviourTree.hpp"
#include "Tunic/ECS/World.hpp"
#include "Tunic/ECS/Components/BehaviourTreeComponent.hpp"

namespace tnc::ecs {
	BehaviourTreeSystem::BehaviourTreeSystem() = default;

	BehaviourTreeSystem::BehaviourTreeSystem(BehaviourTreeSystem&& other) noexcept = default;

	BehaviourTreeSystem& BehaviourTreeSystem::operator=(BehaviourTreeSystem&& other) noexcept = default;

	BehaviourTreeSystem::~BehaviourTreeSystem() = default;

	void BehaviourTreeSystem::update(World& world, clv::utl::DeltaTime deltaTime) {
		for(auto [behaviourTree] : world.getComponentSets<BehaviourTreeComponent>()) {
			behaviourTree->behaviourTree->run();
		}
	}
}