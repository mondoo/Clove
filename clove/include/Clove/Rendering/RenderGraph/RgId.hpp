#pragma once

#include <cinttypes>
#include <cstddef>

namespace clove {
    using RgResourceId = uint32_t;
    using RgPassId     = uint32_t;

    using RgBufferId    = RgResourceId;
    using RgImageId     = RgResourceId;
    using RgImageViewId = RgResourceId;

    static RgResourceId constexpr INVALID_RESOURCE_ID{ 0 };
    static RgPassId constexpr INVALID_PASS_ID{ 0 };
}
