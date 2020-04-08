#pragma once

#include "Tunic/UI/Widget.hpp"

namespace tnc::rnd {
	class Sprite;
}

namespace tnc::ui{
	class Image : public Widget {
		//VARIABLES
	private:
		std::shared_ptr<rnd::Sprite> sprite;

		//FUNCTIONS
	public:
		//TODO: Ctors

		virtual void draw(rnd::Renderer2D& renderer) override;
	};
}