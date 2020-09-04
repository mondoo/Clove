#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class GraphicsDevice;
}

namespace clv::gfx {
    std::unique_ptr<GraphicsDevice> createGraphicsDevice(API api, void* nativeWindow);
}