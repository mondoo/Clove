#pragma once

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace blb::ui {
	class InputElement {
		//FUNCTIONS
	public:
		virtual ~InputElement() = default;

		virtual InputResponse onKeyEvent(const clv::Keyboard::Event& event) = 0;
		virtual InputResponse onMouseEvent(const clv::Mouse::Event& event) = 0;
	};
}