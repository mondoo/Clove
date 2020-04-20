#pragma once

#include "Bulb/AI/Composite.hpp"

namespace blb::ai{
	class Selector : public Composite{
		//FUNCTIONS
	public:
		Selector();

		Selector(const Selector& other) = delete;
		Selector(Selector&& other) noexcept;

		Selector& operator=(const Selector& other) = delete;
		Selector& operator=(Selector&& other) noexcept;

		~Selector();

		virtual Status activate(clv::utl::DeltaTime deltaTime) override;
	};
}