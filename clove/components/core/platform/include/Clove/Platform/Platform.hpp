#pragma once

#include "Clove/Platform/PlatformTypes.hpp"

#include <Clove/Maths/Vector.hpp>
#include <memory>

namespace garlic::clove {
    class Window;
}

namespace garlic::clove {
    class Platform {
        //FUNCTIONS
    public:
        virtual ~Platform() = default;

        virtual std::shared_ptr<Window> createWindow(WindowDescriptor const &props) = 0;
    };
}

namespace garlic::clove {
    std::unique_ptr<Platform> createPlatformInstance();
}