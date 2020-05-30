#include "Bulb/AI/BehaviourTree.hpp"

namespace blb::ai {
	BehaviourTree::BehaviourTree() = default;

	BehaviourTree::BehaviourTree(BehaviourTree&& other) noexcept = default;

	BehaviourTree& BehaviourTree::operator=(BehaviourTree&& other) noexcept = default;

	BehaviourTree::~BehaviourTree() = default;

	void BehaviourTree::setRootTask(std::unique_ptr<Task> task) {
		rootTask = std::move(task);
	}

	BlackBoard& BehaviourTree::BlackBoard(){
		return blackboard;
	}

	void BehaviourTree::run(clv::utl::DeltaTime deltaTime) {
		if(rootTask != nullptr) {
			rootTask->activate(deltaTime, blackboard);
		}
	}
}