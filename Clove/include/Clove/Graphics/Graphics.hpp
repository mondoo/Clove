#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class GraphicsFactory;
}

namespace clv::gfx {
    std::unique_ptr<GraphicsFactory> createGraphicsFactory(API api, void* nativeWindow);
}