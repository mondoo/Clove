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
		//TODO: Ctors
		BTBrain() = delete;
		BTBrain(ai::BehaviourTree behaviourTree);

		void update(const clv::utl::DeltaTime deltaTime) override;
	};
}