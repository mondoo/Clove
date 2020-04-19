#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/GraphicsFactory.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx{
	std::shared_ptr<GraphicsFactory> initialise(API api);
}