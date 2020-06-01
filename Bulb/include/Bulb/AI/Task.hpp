#pragma once

namespace blb::ai {
	class BlackBoard;
}

namespace blb::ai {
	class Task {
		//TYPES
	public:
		enum class Status {
			Running,
			Success,
			Failure
		};

		//FUNCTIONS
	public:
		virtual ~Task() = default;

		virtual Status activate(const clv::utl::DeltaTime deltaTime, BlackBoard& blackboard) = 0;
	};
}