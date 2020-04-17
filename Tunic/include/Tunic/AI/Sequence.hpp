#pragma once

#include "Tunic/AI/Composite.hpp"

namespace tnc::ai {
	class Sequence : public Composite {
		//FUNCTIONS
	public:
		Sequence();

		Sequence(const Sequence& other) = delete;
		Sequence(Sequence&& other) noexcept;

		Sequence& operator=(const Sequence& other) = delete;
		Sequence& operator=(Sequence&& other) noexcept;

		~Sequence();

		virtual Status activate() override;
	};
}