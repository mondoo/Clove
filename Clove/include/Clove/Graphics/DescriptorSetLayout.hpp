#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    enum class DescriptorStage {
        Vertex,
        Pixel
    };

    struct DescriptorSetBindingInfo {
        uint32_t binding{ 0 };
        DescriptorType type;
        size_t arraySize{ 1 };/**< If this descriptor represents an array it'll be the size of the array else 1 */
        DescriptorStage stage;
    };

    /**
     * @brief Describes the layout of an entire DescriptorSet.
     * @details A 'set' is defined in the shader by: layout(set = 0, binding = 1). This
     * layout describes the type of binding in each binding slot for a given set. The index
     * of the set is defined in the array of DescriptorSetLayouts given to the 
     * PiplineObject::Descriptor.
     */
    class DescriptorSetLayout {
        //TYPES
    public:
        struct Descriptor {
            std::vector<DescriptorSetBindingInfo> bindings;
        };

        //FUNCTIONS
    public:
        virtual ~DescriptorSetLayout() = default;

        virtual const Descriptor& getDescriptor() const = 0;
    };
}