#pragma once

#include "Clove/Graphics/PipelineObject.hpp"

#include <vector>

namespace clove {
    class GhaDescriptorSetLayout;
    class GhaShader;
}

namespace clove {
    class GhaComputePipelineObject {
        //TYPES
    public:
        struct Descriptor {
            GhaShader const *shader{ nullptr };

            std::vector<GhaDescriptorSetLayout const *> descriptorSetLayouts;
            std::vector<PushConstantDescriptor> pushConstants;
        };

        //FUNCTIONS
    public:
        virtual ~GhaComputePipelineObject() = default;

        virtual Descriptor const &getDescriptor() const = 0;
    };
}