#pragma once

#include "Tunic/AI/Task.hpp"

namespace tnc::ai {
	class Decorator : public Task {
		//VARIABLES
	protected:
		std::unique_ptr<Task> child;

		//FUNCTIONS
	public:
		Decorator();

		Decorator(const Decorator& other) = delete;
		Decorator(Decorator&& other) noexcept;

		Decorator& operator=(const Decorator& other) = delete;
		Decorator& operator=(Decorator&& other) noexcept;

		virtual ~Decorator();

		void addChild(std::unique_ptr<Task> child);
	};
}