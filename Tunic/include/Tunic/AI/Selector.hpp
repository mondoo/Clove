#pragma once

#include "Tunic/AI/Composite.hpp"

namespace tnc::ai{
	class Selector : public Composite{
		//FUNCTIONS
	public:
		Selector();

		Selector(const Selector& other) = delete;
		Selector(Selector&& other) noexcept;

		Selector& operator=(const Selector& other) = delete;
		Selector& operator=(Selector&& other) noexcept;

		~Selector();

		virtual Status activate() override;
	};
}