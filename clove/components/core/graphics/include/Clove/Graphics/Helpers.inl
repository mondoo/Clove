#if CLOVE_GHA_VALIDATION
    #include "Clove/Graphics/Validation/ValidationBuffer.hpp"
    #include "Clove/Graphics/Validation/ValidationGraphicsCommandBuffer.hpp"
    #include "Clove/Graphics/Validation/ValidationGraphicsQueue.hpp"

    #include <type_traits>
#endif

namespace clove {
    template<typename GhaObjectType, typename... Args>
    std::unique_ptr<GhaObjectType> createGhaObject(Args &&...args) {
#if CLOVE_GHA_VALIDATION
        if constexpr(std::is_base_of_v<GhaBuffer, GhaObjectType>) {
            return std::make_unique<ValidationBuffer<GhaObjectType>>(std::forward<Args>(args)...);
        } else if constexpr(std::is_base_of_v<GhaGraphicsQueue, GhaObjectType>) {
            return std::make_unique<ValidationGraphicsQueue<GhaObjectType>>(std::forward<Args>(args)...);
        } else if constexpr(std::is_base_of_v<GhaGraphicsCommandBuffer, GhaObjectType>) {
            return std::make_unique<ValidationGraphicsCommandBuffer<GhaObjectType>>(std::forward<Args>(args)...);
        } else {
            return std::make_unique<GhaObjectType>(std::forward<Args>(args)...);
        }
#else
        return std::make_unique<GhaObjectType>(std::forward<Args>(args)...);
#endif
    }
}