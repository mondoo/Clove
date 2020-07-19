#pragma once

#include "Bulb/Controller/Controller.hpp"

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/InputEvent.hpp>

namespace blb {
	/**
	 * @brief Represents a player.
	 *
	 * Abstracts away certain player specific functionality (such as input) so various different
	 * types of Entities can be controlled with only this single class handling input.
	 */
	class PlayerController : public Controller {
		//FUNCTIONS
	public:
		virtual ~PlayerController() = default;

		virtual InputResponse handleInput(const clv::InputEvent& inputEvent) = 0;
	};
}
