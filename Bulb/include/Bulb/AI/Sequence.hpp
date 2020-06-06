#pragma once

#include "Bulb/AI/Composite.hpp"

namespace blb::ai {
	class Sequence : public Composite {
		//FUNCTIONS
	public:
		Sequence();

		Sequence(const Sequence& other) = delete;
		Sequence(Sequence&& other) noexcept;

		Sequence& operator=(const Sequence& other) = delete;
		Sequence& operator=(Sequence&& other) noexcept;

		~Sequence();

		Status activate(const clv::utl::DeltaTime deltaTime, BlackBoard& blackboard) override;
	};
}