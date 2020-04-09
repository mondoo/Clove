#pragma once

#include "Clove/Graphics/Core/FactoryChild.hpp"

namespace clv::gfx {
	class RenderTarget : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~RenderTarget() = default;

		virtual void clear() = 0;

		virtual void setClearColour(const mth::vec4f& colour) = 0;
	};
}
