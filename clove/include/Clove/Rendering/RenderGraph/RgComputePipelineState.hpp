#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

namespace garlic::clove {
    class RgComputePipelineState {
        //TYPES
    public:
        struct Descriptor {
            RgShader computeShader;
        };
        
        //VARIABLES
    private:
        ResourceIdType id{ INVALID_ID };

        //FUNCTIONS
    public:
        operator ResourceIdType() {
            return id;
        }
    };
}