#pragma once

#include "Bulb/Controller/Controller.hpp"

#include "Bulb/AI/BehaviourTree.hpp"

namespace blb {
	/**
	 * @brief Repsrents the behaviour of an AI that controlls an Entity.
	 *
	 * The BTAIController uses a BehaviourTree as it's decision making mechanism.
	 */
	class BTAIController : public Controller {
		//VARIABLES
	private:
		ai::BehaviourTree behaviourTree;
		ai::BlackBoard::Key possessedEntityKey;

		//FUNCTIONS
	public:
		BTAIController() = delete;
		/**
		 * @param behaviourTree The BehaviourTree that determins this Controller's behaviour.
		 * @param possessedEntityKey The key that get's set on the BehaviourTree's BlackBoard so Tasks can query it to get the Entity that is possessed by this Controller.
		 */
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
