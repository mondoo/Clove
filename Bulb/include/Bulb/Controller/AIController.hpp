#pragma once

#include "Bulb/Controller/Brain.hpp"
#include "Bulb/Controller/Controller.hpp"

namespace blb {
	class AIController : public Controller {
		//VARIABLES
	private:
		std::unique_ptr<Brain> brain;

		//FUNCTIONS
	public:
		//TODO: Cotrs
		AIController() = delete;
		AIController(std::unique_ptr<Brain> brain);

		void possess(blb::ecs::Entity entity) override;

		void update(const clv::utl::DeltaTime deltaTime);
	};
}