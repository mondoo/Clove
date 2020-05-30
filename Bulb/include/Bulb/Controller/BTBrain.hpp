#pragma once

#include "Bulb/AI/BehaviourTree.hpp"
#include "Bulb/Controller/Brain.hpp"

namespace blb {
	class BTBrain : public Brain {
		//VARIABLES
	private:
		ai::BehaviourTree behaviourTree;

		//FUNCTIONS
	public:
		BTBrain() = delete;
		BTBrain(ai::BehaviourTree behaviourTree);

		BTBrain(const BTBrain& other) = delete;
		BTBrain(BTBrain&& other) noexcept;

		BTBrain& operator=(const BTBrain& other) = delete;
		BTBrain& operator=(BTBrain&& other) noexcept;

		~BTBrain();

		void update(const clv::utl::DeltaTime deltaTime) override;
	};
}