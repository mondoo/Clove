#pragma once

#include "Bulb/Controller/Controller.hpp"

#include "Bulb/AI/BehaviourTree.hpp"

namespace blb {
	class BTAIController : public Controller {
		//VARIABLES
	private:
		ai::BehaviourTree behaviourTree;
		ai::BlackBoard::Key possessedEntityKey;

		//FUNCTIONS
	public:
		BTAIController() = delete;
		BTAIController(ai::BehaviourTree behaviourTree, ai::BlackBoard::Key possessedEntityKey);

		BTAIController(const BTAIController& other) = delete;
		BTAIController(BTAIController&& other) noexcept;

		BTAIController& operator=(const BTAIController& other) = delete;
		BTAIController& operator=(BTAIController&& other) noexcept;

		~BTAIController();

		void possess(blb::ecs::Entity entity) override;

		void update(const clv::utl::DeltaTime deltaTime);
	};
}