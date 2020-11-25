#pragma once

#include "Clove/Graphics/GraphicsAPI.hpp"

#include <any>
#include <optional>
#include <memory>

namespace garlic::clove {
    class GraphicsDevice;
}

namespace garlic::clove {
    /**
     * @brief Create a Graphics Device to communicate with the graphics hardware.
     * @param api What API to use.
     * @param nativeWindow The window to bind to. Using an empty std::any will create a headless device.
     * @return 
     */
    std::unique_ptr<GraphicsDevice> createGraphicsDevice(GraphicsApi api, std::any nativeWindow);
}