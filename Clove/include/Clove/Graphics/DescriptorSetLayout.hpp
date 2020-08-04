#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    enum class DescriptorStage {
        Vertex,
        Pixel
    };

    struct DescriptorBindingInfo {
        uint32_t binding{ 0 };
        DescriptorType type;
        size_t arraySize{ 1 };/**< If this descriptor represents an array it'll be the size of the array else 1 */
        DescriptorStage stage;
    };

    /**
     * @brief Describes all of the bindings that the shaders will use.
     */
    class DescriptorSetLayout {
        //TYPES
    public:
        struct Descriptor {
            std::vector<DescriptorBindingInfo> bindings;
        };

        //FUNCTIONS
    public:
        virtual ~DescriptorSetLayout() = default;
    };
}