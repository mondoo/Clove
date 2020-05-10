#pragma once

namespace blb::rnd {
	class Renderer3D;
}

namespace blb::ui {
	class DrawableElement {
		//FUNCTIONS
	public:
		virtual ~DrawableElement() = default;

		virtual void draw(rnd::Renderer3D& renderer, const clv::mth::vec2f& drawSpace) = 0;
	};
}