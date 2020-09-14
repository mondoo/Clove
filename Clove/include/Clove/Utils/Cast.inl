#include <Root/Definitions.hpp>

#include "Clove/Log.hpp"

namespace clv {
    template<typename DestType, typename SourceType>
    DestType* polyCast(SourceType* source) {
#if GARLIC_DEBUG
        auto* result = dynamic_cast<DestType*>(source);
        GARLIC_ASSERT(result != nullptr, "Cast failed");
        return result;
#else
        return static_cast<DestType*>(source);
#endif
    }
}