#pragma once

#include <cinttypes>
#include <cstddef>

namespace clove{
    using RgResourceIdType = uint32_t;
    using RgPassIdType     = uint32_t;

    static RgResourceIdType constexpr INVALID_RESOURCE_ID{ 0 };
    static RgPassIdType constexpr INVALID_PASS_ID{ 0 };
}
