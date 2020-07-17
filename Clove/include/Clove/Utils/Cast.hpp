#pragma once

namespace clv {
	template<typename DestType, typename SourceType>
    DestType* polyCast(SourceType* source);
}

#include "Cast.inl"