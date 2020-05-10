#pragma once

#include "Bulb/InputResponse.hpp"
#include <Clove/Input/InputEvent.hpp>

namespace blb::rnd {
	class Renderer3D;
}

namespace blb::ui {
	class UIElement;
}

namespace blb::ui {
	class UIFrame {
		//VARIABLES
	private:
		//SharedPtr for now, good candidate for an allocator to get cache friendly
		std::vector<std::shared_ptr<UIElement>> elements;

		clv::mth::vec2i size;

		//FUNCTIONS
	public:
		UIFrame() = delete;
		UIFrame(clv::mth::vec2i size);

		InputResponse propogateInput(const clv::InputEvent& event);

		void drawChildren(rnd::Renderer3D& renderer);
	};
}