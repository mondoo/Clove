#pragma once

#include "Bulb/AI/Task.hpp"

namespace blb::ai {
	class BehaviourTree {
		//VARIABLES
	private:
		std::unique_ptr<Task> rootTask;

		//FUNCTIONS
	public:
		BehaviourTree();

		BehaviourTree(const BehaviourTree& other) = delete;
		BehaviourTree(BehaviourTree&& other) noexcept;

		BehaviourTree& operator=(const BehaviourTree& other) = delete;
		BehaviourTree& operator=(BehaviourTree&& other) noexcept;

		~BehaviourTree();

		void setRootTask(std::unique_ptr<Task> task);

		void run(clv::utl::DeltaTime deltaTime);
	};
}