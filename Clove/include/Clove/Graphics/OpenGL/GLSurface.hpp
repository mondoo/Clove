#pragma once

#include "Clove/Core/Graphics/Surface.hpp"

namespace clv::gfx::ogl{
	class GLSurface : public Surface{
		//FUNCTIONS
	public:
		virtual ~GLSurface() = default;

		virtual void makeCurrent() = 0;

		virtual void resizeBuffers(const mth::vec2ui& size) override{};
	};
}