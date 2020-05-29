#include "Bulb/Controller/AIController.hpp"

namespace blb {
	AIController::AIController(std::unique_ptr<Brain> brain)
		: brain(std::move(brain)) {
	}

	void AIController::possess(ecs::Entity entity) {
		//TODO: Need some kind of blackboard system so the controller can update the entitiy field on it
	}

	void AIController::update(const clv::utl::DeltaTime deltaTime) {
		brain->update(deltaTime);
	}
}
