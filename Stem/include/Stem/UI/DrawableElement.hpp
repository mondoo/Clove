#pragma once

namespace garlic::inline stem {
    class ForwardRenderer3D;
}

namespace garlic::inline stem {
	class DrawableElement {
		//FUNCTIONS
	public:
		virtual ~DrawableElement() = default;

        virtual void draw(ForwardRenderer3D &renderer, const clv::mth::vec2f &drawSpace) = 0;
    };
}