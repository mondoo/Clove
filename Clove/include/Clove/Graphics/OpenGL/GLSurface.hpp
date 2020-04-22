#pragma once

#include "Clove/Graphics/Surface.hpp"

namespace clv::gfx::ogl{
	class GLSurface : public Surface{
		//FUNCTIONS
	public:
		void resizeBuffers(const mth::vec2ui& size) override{};
	};
}