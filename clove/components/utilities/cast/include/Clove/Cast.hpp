#pragma once

namespace garlic::clove {
    template<typename DestType, typename SourceType>
    DestType *polyCast(SourceType *source);
}

#include "Cast.inl"