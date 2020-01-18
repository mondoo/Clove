#pragma once

#include "Clove/Graphics/Core/Surface.hpp"

namespace clv::gfx::ogl{
	class GLSurface : public Surface{
		//FUNCTIONS
	public:
		virtual ~GLSurface() = default;

		virtual void resizeBuffers(const mth::vec2ui& size) override{};
	};
}