#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderAPI{
		//FUNCTIONS
	public:
		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;
	};
}