#pragma once

namespace garlic::inline stem {
	class DrawableElement {
		//FUNCTIONS
	public:
		virtual ~DrawableElement() = default;

        virtual void draw(const clv::mth::vec2f &drawSpace) = 0;
    };
}