#pragma once

namespace blb::rnd {
    class ForwardRenderer3D;
}

namespace blb::ui {
	class DrawableElement {
		//FUNCTIONS
	public:
		virtual ~DrawableElement() = default;

        virtual void draw(rnd::ForwardRenderer3D &renderer, const clv::mth::vec2f &drawSpace) = 0;
    };
}