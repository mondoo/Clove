#include "Bulb/Controller/BTAIController.hpp"

namespace blb {
	BTAIController::BTAIController(ai::BehaviourTree behaviourTree, ai::BlackBoard::Key possessedEntityKey)
		: behaviourTree(std::move(behaviourTree))
		, possessedEntityKey(possessedEntityKey){
	}

	BTAIController::BTAIController(BTAIController&& other) noexcept = default;

	BTAIController &BTAIController::operator=(BTAIController &&other) noexcept = default;

	BTAIController::~BTAIController() = default;

	void BTAIController::possess(blb::ecs::Entity entity) {
		behaviourTree.getBlackBoard().setValue(possessedEntityKey, entity);
	}

	void BTAIController::update(const clv::utl::DeltaTime deltaTime) {
		behaviourTree.run(deltaTime);
	}
}


