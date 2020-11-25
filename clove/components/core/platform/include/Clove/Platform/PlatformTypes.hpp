#pragma once

#include <string>
#include <cinttypes>

namespace garlic::clove {
    struct WindowDescriptor {
        std::string title;
        int32_t width;
        int32_t height;
    };
}