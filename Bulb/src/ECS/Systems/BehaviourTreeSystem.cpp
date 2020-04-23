#include "Bulb/ECS/Systems/BehaviourTreeSystem.hpp"

#include "Bulb/AI/BehaviourTree.hpp"
#include "Bulb/ECS/World.hpp"
#include "Bulb/ECS/Components/BehaviourTreeComponent.hpp"

namespace blb::ecs {
	BehaviourTreeSystem::BehaviourTreeSystem() = default;

	BehaviourTreeSystem::BehaviourTreeSystem(BehaviourTreeSystem&& other) noexcept = default;

	BehaviourTreeSystem& BehaviourTreeSystem::operator=(BehaviourTreeSystem&& other) noexcept = default;

	BehaviourTreeSystem::~BehaviourTreeSystem() = default;

	void BehaviourTreeSystem::update(World& world, clv::utl::DeltaTime deltaTime) {
		for(auto&& [btComp] : world.getComponentSets<BehaviourTreeComponent>()) {
			if(ai::BehaviourTree* bt = btComp->behaviourTree.get()) {
				bt->run(deltaTime);
			}
		}
	}
}