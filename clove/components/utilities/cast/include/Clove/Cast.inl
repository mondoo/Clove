#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    template<typename DestType, typename SourceType>
    DestType *polyCast(SourceType *source) {
#if GARLIC_DEBUG
        auto *result = dynamic_cast<DestType *>(source);
        GARLIC_ASSERT(result != nullptr, "Cast failed");
        return result;
#else
        return static_cast<DestType *>(source);
#endif
    }
}