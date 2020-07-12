#pragma once

namespace blb::ai {
	class BlackBoard;
}

namespace blb::ai {
    /**
	 * @brief a Task is an action an AI agent can perform. Determined by a behaviour tree
	 */
	class Task {
		//TYPES
	public:
        /**
		 * @brief Represents the status of a task after activating it
		 */
		enum class Status {
            Running, /**< Signifies this task is still running. The tree will re-traverse itself up to this task next update */
            Success, /**< Signifies that this task finished successfully */
			Failure  /**< Signifies that this task did not finished successfully */
		};

		//FUNCTIONS
	public:
		virtual ~Task() = default;

		virtual Status activate(const clv::utl::DeltaTime deltaTime, BlackBoard& blackboard) = 0;
	};
}