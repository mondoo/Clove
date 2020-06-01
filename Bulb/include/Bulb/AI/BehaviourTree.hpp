#pragma once

#include "Bulb/AI/BlackBoard.hpp"
#include "Bulb/AI/Task.hpp"

namespace blb::ai {
	class BehaviourTree {
		//VARIABLES
	private:
		BlackBoard blackboard;
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

		BlackBoard& getBlackBoard();

		void run(clv::utl::DeltaTime deltaTime);
	};
}