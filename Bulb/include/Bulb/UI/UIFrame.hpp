#pragma once

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/InputEvent.hpp>

namespace blb::rnd {
	class Renderer3D;
}

namespace blb::ui {
	class DrawableElement;
	class InputElement;
}

namespace blb::ui {
	class UIFrame {
		//VARIABLES
	private:
		//SharedPtr for now, good candidate for an allocator to get cache friendly
		std::vector<std::shared_ptr<DrawableElement>> drawables;
		std::vector<std::shared_ptr<InputElement>> inputElements;

		clv::mth::vec2i size;

		//FUNCTIONS
	public:
		UIFrame() = delete;
		UIFrame(clv::mth::vec2i size);

		InputResponse propagateInput(const clv::InputEvent& event);

		void drawChildren(rnd::Renderer3D& renderer);

		template<typename Element>
		void addElement(std::shared_ptr<Element> element) {
			if constexpr(std::is_base_of_v<DrawableElement, Element>) {
				drawables.push_back(element);
			}
			if constexpr(std::is_base_of_v<InputElement, Element>) {
				inputElements.push_back(element);
			}
		}
	};
}