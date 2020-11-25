#pragma once

#include <cinttypes>
#include <cstddef>

namespace garlic::clove {
    using EntityID    = uint32_t;
    using ComponentID = size_t;

    inline constexpr EntityID INVALID_ENTITY_ID{ 0 };
}