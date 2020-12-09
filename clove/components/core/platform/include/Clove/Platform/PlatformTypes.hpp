#pragma once

#include <cinttypes>
#include <string>

namespace garlic::clove {
    struct WindowDescriptor {
        std::string title;
        int32_t width{ 800 };
        int32_t height{ 600 };
    };
}