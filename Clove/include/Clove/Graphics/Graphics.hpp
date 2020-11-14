#pragma once

#include "Clove/Graphics/GraphicsAPI.hpp"

#include <any>

namespace clv::gfx {
    class GraphicsDevice;
}

namespace clv::gfx {
    std::unique_ptr<GraphicsDevice> createGraphicsDevice(API api, std::any nativeWindow);
}