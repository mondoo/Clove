#pragma once

#include "Tunic/AI/Task.hpp"

namespace tnc::ai {
	class Composite : public Task {
		//VARIABLES
	protected:
		std::vector<std::unique_ptr<Task>> children;

		//FUNCTIONS
	public:
		Composite();

		Composite(const Composite& other) = delete;
		Composite(Composite&& other) noexcept;

		Composite& operator=(const Composite& other) = delete;
		Composite& operator=(Composite&& other) noexcept;
		
		virtual ~Composite();

		void addChild(std::unique_ptr<Task> child);
	};
}