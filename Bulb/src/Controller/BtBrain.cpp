#include "Bulb/Controller/BTBrain.hpp"

namespace blb {
	BTBrain::BTBrain(ai::BehaviourTree behaviourTree)
		: behaviourTree(std::move(behaviourTree)) {
	}

	void BTBrain::update(const clv::utl::DeltaTime deltaTime) {
		behaviourTree.run(deltaTime);
	}
}