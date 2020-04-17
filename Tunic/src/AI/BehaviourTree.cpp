#include "Tunic/AI/BehaviourTree.hpp"

namespace tnc::ai {
	BehaviourTree::BehaviourTree() = default;

	BehaviourTree::BehaviourTree(BehaviourTree&& other) noexcept = default;

	BehaviourTree& BehaviourTree::operator=(BehaviourTree&& other) noexcept = default;

	BehaviourTree::~BehaviourTree() = default;

	void BehaviourTree::setRootTask(std::unique_ptr<Task> task) {
		rootTask = std::move(task);
	}

	void BehaviourTree::run() {
		if(rootTask != nullptr) {
			rootTask->activate();
		}
	}
}