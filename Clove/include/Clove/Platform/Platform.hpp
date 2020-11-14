#pragma once

#include "Clove/Platform/PlatformTypes.hpp"

namespace clv::gfx {
    class GraphicsFactory;
}

namespace clv::plt {
    class Window;
}

namespace clv::plt {
    class Platform {
        //FUNCTIONS
    public:
        virtual ~Platform() = default;

        virtual std::shared_ptr<Window> createWindow(WindowDescriptor const &props)                                                       = 0;
        virtual std::shared_ptr<Window> createChildWindow(Window const &parentWindow, mth::vec2i const &position, mth::vec2i const &size) = 0;
    };
}

namespace clv::plt {
    std::unique_ptr<Platform> createPlatformInstance();
}