#pragma once

namespace tnc::ai{
	enum class TaskStatus{
		running,
		success,
		failure
	};
}

namespace tnc::ai {
	class Task {
		//FUNCTIONS
	public:
		virtual ~Task() = default;

		virtual TaskStatus activate() = 0;
	};
}