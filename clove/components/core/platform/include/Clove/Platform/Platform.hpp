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
        static std::unique_ptr<Window> createWindow(WindowDescriptor const &descriptor);
    };
}
