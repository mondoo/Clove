#pragma once

#include "Tunic/UI/Widget.hpp"

namespace clv::gfx {
	class GraphicsFactory;
}

namespace tnc::rnd {
	class Sprite;
}

namespace tnc::ui {
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