#include "Bulb/Controller/BTAIController.hpp"

namespace blb {
	BTAIController::BTAIController(ai::BehaviourTree behaviourTree)
		: behaviourTree(std::move(behaviourTree)){
	}

	BTAIController::BTAIController(BTAIController&& other) noexcept = default;

	BTAIController &BTAIController::operator=(BTAIController &&other) noexcept = default;

	BTAIController::~BTAIController() = default;

	void BTAIController::possess(blb::ecs::Entity entity) {
		//behaviourTree.getBlackBoard().
	}

	void BTAIController::update(const clv::utl::DeltaTime deltaTime) {
		behaviourTree.run(deltaTime);
	}
}


