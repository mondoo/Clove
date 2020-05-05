#pragma once

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

		virtual Status activate(clv::utl::DeltaTime deltaTime) = 0;
	};
}