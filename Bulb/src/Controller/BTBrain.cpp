#include "Bulb/Controller/BTBrain.hpp"

namespace blb {
	BTBrain::BTBrain(ai::BehaviourTree behaviourTree)
		: behaviourTree(std::move(behaviourTree)) {
	}

	BTBrain::BTBrain(BTBrain&& other) noexcept = default;

	BTBrain& BTBrain::operator=(BTBrain&& other) noexcept = default;

	BTBrain::~BTBrain() = default;

	void BTBrain::update(const clv::utl::DeltaTime deltaTime) {
		behaviourTree.run(deltaTime);
	}
}