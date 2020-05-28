#pragma once

#include "Bulb/Controller/Controller.hpp"

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/InputEvent.hpp>

namespace blb {
	class PlayerController : public Controller {
		//FUNCTIONS
	public:
		virtual ~PlayerController() = default;

		virtual InputResponse handleInput(const clv::InputEvent& inputEvent) = 0;
	};
}