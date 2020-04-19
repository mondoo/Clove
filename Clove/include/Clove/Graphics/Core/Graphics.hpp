#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/GraphicsFactory.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx{
	std::shared_ptr<GraphicsFactory> initialise(API api);
}