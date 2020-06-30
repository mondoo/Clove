#pragma once

namespace clv {
	template<typename DestType, typename SourceType>
    DestType* safeCast(SourceType* source);
}

#include "Cast.inl"