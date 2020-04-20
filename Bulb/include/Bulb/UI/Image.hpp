#pragma once

#include "Bulb/UI/Widget.hpp"

namespace clv::gfx {
	class GraphicsFactory;
}

namespace blb::rnd {
	class Sprite;
}

namespace blb::ui {
	class Image : public Widget {
		//VARIABLES
	//private:
	public:
		std::shared_ptr<rnd::Sprite> sprite;

		//FUNCTIONS
	public:
		Image(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		//TODO: Ctors

		virtual void draw(rnd::Renderer2D& renderer, const clv::mth::vec2f& drawSpace) override;
	};
}