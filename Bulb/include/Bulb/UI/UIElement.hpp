#pragma once

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace blb::rnd {
	class Renderer3D;
}

namespace blb::ui {
	class UIElement {
		//FUNCTIONS
	public:
		virtual ~UIElement() = default;

		virtual InputResponse onKeyEvent(const clv::Keyboard::Event& event) = 0;
		virtual InputResponse onMouseEvent(const clv::Mouse::Event& event) = 0;

		virtual void draw(rnd::Renderer3D& renderer, const clv::mth::vec2f& drawSpace) = 0;
	};
}