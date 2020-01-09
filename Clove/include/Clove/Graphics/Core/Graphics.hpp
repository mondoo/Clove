#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

namespace clv::gfx{
	class RenderFactory;
}

namespace clv::gfx{
	std::unique_ptr<RenderFactory> initialise(API api);
}