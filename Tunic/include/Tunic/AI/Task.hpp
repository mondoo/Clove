#pragma once

namespace tnc::ai {
	class Task {
		//TYPES
	public:
		enum class Status {
			running,
			success,
			failure
		};

		//FUNCTIONS
	public:
		virtual ~Task() = default;

		virtual Status activate() = 0;
	};
}