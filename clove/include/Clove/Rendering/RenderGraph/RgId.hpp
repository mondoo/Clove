#pragma once

#include <cinttypes>
#include <cstddef>

namespace garlic::clove{
    using ResourceIdType = size_t;
    using PassIdType     = size_t;

    static ResourceIdType constexpr INVALID_RESOURCE_ID{ 0 };
    static PassIdType constexpr INVALID_PASS_ID{ 0 };
}
